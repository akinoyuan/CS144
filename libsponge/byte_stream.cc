#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    _capacity = capacity;
    _buff.resize(capacity);
}

size_t ByteStream::write(const string &data) {
    size_t acc = 0;
    for(const auto &i : data){
        if(_nwrite >= _nread + _capacity) break;
        _buff[_nwrite%_capacity] = i;
        ++_nwrite;
        ++acc;
    }
    return acc;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string s = "";
    for(size_t i=0; i<len; i++){
        if(_nread+i==_nwrite) break;
        s.push_back(_buff[(_nread+i)%_capacity]);
    }
    return s;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    for(size_t i=0; i<len; i++){
        if(_nread==_nwrite) return;
        ++_nread;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string s = peek_output(len);
    pop_output(len);
    return s;
}

void ByteStream::end_input() { _eof = true;}

bool ByteStream::input_ended() const { return _eof; }

size_t ByteStream::buffer_size() const { return _nwrite - _nread; }

bool ByteStream::buffer_empty() const { return _nwrite == _nread; }

bool ByteStream::eof() const { return _eof && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _nwrite; }

size_t ByteStream::bytes_read() const { return _nread; }

size_t ByteStream::remaining_capacity() const { return _capacity - buffer_size(); }
