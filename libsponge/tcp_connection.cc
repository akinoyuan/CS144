#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return _time_since_last_segment_received; }

void TCPConnection::segment_received(const TCPSegment &seg) {
    if(!_active) return;                    //连接已终止
    _time_since_last_segment_received = 0;
    if(seg.header().rst) {                  //收到设置RST的segment
        _unclean_shutdown();
        return;
    }
    _receiver.segment_received(seg);        //receiver接收seg
    if(seg.header().ack) {                  //ackno和win传给sender
        _sender.ack_received(seg.header().ackno, seg.header().win);
    }
    if(seg.length_in_sequence_space()>0) {  //seg不为空,至少发送一个回复
        _sender.fill_window();
        if(!_send_segments()){              //未能发送成功，发送一个空的seg
            _sender.send_empty_segment();
            _send_segments();
        }
    }
    if (_receiver.ackno().has_value() && (seg.length_in_sequence_space() == 0) //回复 “keep-alive” segment
    && seg.header().seqno == _receiver.ackno().value() - 1) {
        _sender.send_empty_segment();
        _send_segments();
    }
}

bool TCPConnection::active() const { return _active; }

size_t TCPConnection::write(const string &data) {
    size_t write_num = _sender.stream_in().write(data);     //写入的字节数
    _sender.fill_window();
    _send_segments();
    return write_num;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
    _time_since_last_segment_received += ms_since_last_tick;    //计时功能
    _sender.tick(ms_since_last_tick);                           //告诉sender过去的时间
    if (_sender.segments_out().size() > 0) {                    //发生超时重传
        //重传次数超过限制，关闭连接并发送RST给对方
        if (_sender.consecutive_retransmissions() > _cfg.MAX_RETX_ATTEMPTS) {
            _unclean_shutdown();
            TCPSegment rst_seg;       //an empty segment with the rst flag set
            rst_seg.header().rst = true;
            _segments_out.push(rst_seg);
        }else{
            _send_segments();
        }
    }
    _clean_shutdown();  //是否到达lingering的时间
}

void TCPConnection::end_input_stream() {
    _sender.stream_in().end_input();
    _sender.fill_window();
    _send_segments();
}

void TCPConnection::connect() {
    _sender.fill_window();              //设置一个带SYN的segment
    _send_segments();
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}

//由TCPConnection真正的发送segment,这个segment中同时包含sender和receiver的信息，
//返回是否发送出去至少一个seg
bool TCPConnection::_send_segments() {
    bool real_send = false;
    while(!_sender.segments_out().empty()) {    //sender的输出队列不为空
        real_send = true;
        TCPSegment seg = _sender.segments_out().front();
        _sender.segments_out().pop();
        if(_receiver.ackno().has_value()){      //segment首部加上ackno和windowsize字段，还有ACK标志位
            seg.header().ack = true;
            seg.header().ackno = _receiver.ackno().value();
        }
        seg.header().win = _receiver.window_size();
        _segments_out.push(seg);
    }
    _clean_shutdown();                  //每次传输完毕后检查是否可以干净的终止连接
    return real_send;
}

void TCPConnection::_clean_shutdown() {
    if(_receiver.stream_out().input_ended()) {  //先决条件#1
        if(!_sender.stream_in().eof()) {        //输入流比输出流先结束，即远端先完成输出，lingering设为false
            _linger_after_streams_finish = false;
        }else if(_sender.bytes_in_flight() == 0){   //先决条件#2#3 sender发送了FIN且全部得到确认
            if(!_linger_after_streams_finish || _time_since_last_segment_received >= 10*_cfg.rt_timeout) { //先决条件#4
                _active = false;
            }
        }
    }
}

void TCPConnection::_unclean_shutdown() {
    _sender.stream_in().set_error();
    _receiver.stream_out().set_error();
    _active = false;
}
