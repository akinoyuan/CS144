#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>
#include<iostream>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , _RTO{retx_timeout} {}

uint64_t TCPSender::bytes_in_flight() const { return _bytes_in_flight; }

void TCPSender::fill_window() {
    if(_fin_sent) return;               //已结束
    if(!_syn_sent){                     //发送syn
        TCPSegment seg = TCPSegment();
        seg.header().syn = true;
        _syn_sent = true;
        seg.header().seqno = wrap(_next_seqno,_isn);
        _sendSegments(seg);
        return;
    }
    if(_stream.eof()&&bytes_in_flight()<_window_size){                      //输入结束（上一条segment放不下FIN
        TCPSegment seg = TCPSegment();
        seg.header().fin = true;
        _fin_sent = true;
        seg.header().seqno = wrap(_next_seqno,_isn);
        _sendSegments(seg);
        return;
    }
    while(bytes_in_flight()<_window_size&&!_stream.buffer_empty()){ //输入流不为空，接收窗口未满
        TCPSegment seg = TCPSegment();
        size_t len = min(_window_size-bytes_in_flight(), TCPConfig::MAX_PAYLOAD_SIZE); //?这几个无符号数减下来会不会溢出？
        seg.header().seqno = wrap(_next_seqno,_isn);
        seg.payload() = _stream.read(len);
        if(_stream.eof()&&_window_size-bytes_in_flight()>=seg.length_in_sequence_space()+1){    //输入结束且空间足以放入fin
            seg.header().fin = true;
            _fin_sent = true;
        }
        _sendSegments(seg);
    }
}

void TCPSender::_sendSegments(TCPSegment seg) {
    _next_seqno += seg.length_in_sequence_space();
    _bytes_in_flight += seg.length_in_sequence_space();
    _segments_out.push(seg);
    _outstanding_segments.push(seg);
    if(!_timer_on){
        _time_passed = 0;
        _timer_on = true;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    if(window_size==0){
        _window_size = 1;
        _backoff = false;
    }else{
        _window_size = window_size;
        _backoff = true;
    }
    //首先判断收到的ackno是否有效
    if(_outstanding_segments.empty()) return;
    uint64_t abs_ackno = unwrap(ackno, _isn, _next_seqno);
    TCPSegment front_seg = _outstanding_segments.front();
    uint64_t front_abs_seqno = unwrap(front_seg.header().seqno, _isn, _next_seqno);
    if(!(abs_ackno<=_next_seqno && abs_ackno>=front_abs_seqno)) return;
    while(!_outstanding_segments.empty()){
        front_seg = _outstanding_segments.front();
        front_abs_seqno = unwrap(front_seg.header().seqno, _isn, _next_seqno);
        if(abs_ackno>=front_abs_seqno+front_seg.length_in_sequence_space()){  //确认发送窗口中的至少一整段seg
            _outstanding_segments.pop();
            _RTO = _initial_retransmission_timeout;     //RTO恢复初始值
            _consecutive_retransmissions = 0;           //连续重传置零
            _time_passed = 0;                           //timepassed置零
            _bytes_in_flight -= front_seg.length_in_sequence_space(); 
        }else break;
    }
    if(_outstanding_segments.empty()){                 //传输中队列为空，关闭计时器
        _timer_on = false;
        _time_passed = 0;
    }
    fill_window();                                      //window_size更新后，调用fill_window()传输数据
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    if(_timer_on){
        _time_passed += ms_since_last_tick;
        if(_time_passed >= _RTO){                       //超时处理
            _segments_out.push(_outstanding_segments.front());
            if(_backoff){                               //指数避退
                ++_consecutive_retransmissions;
                _RTO *= 2;
            }
            _time_passed = 0;
        }
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return _consecutive_retransmissions; }

void TCPSender::send_empty_segment() {
    TCPSegment seg = TCPSegment();
    seg.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.push(seg);
}
