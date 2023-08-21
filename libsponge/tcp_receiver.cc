#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    TCPHeader header = seg.header();
    if(header.syn&&_syn)  return;     //收到重复的syn,达咩
    if(header.syn){                   //这是第一个数据包
        _syn = true;
        _isn = header.seqno.raw_value();
        _reassembler.push_substring(seg.payload().copy(), 0, header.fin);
    }else{
        uint64_t abs_seqno = unwrap(header.seqno, WrappingInt32(_isn), stream_out().bytes_written());
        _reassembler.push_substring(seg.payload().copy(), abs_seqno-1, header.fin);
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {    //希望收到的下一个字节的seqno
    if(!_syn)   return nullopt;
    WrappingInt32 ack_no = wrap(stream_out().bytes_written()+1, WrappingInt32(_isn));
	return stream_out().input_ended() ? ack_no+1 : ack_no;	
}

size_t TCPReceiver::window_size() const { return _capacity - stream_out().buffer_size(); }
