#include<iostream>
#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(eof){
        _eof = true;
        _eof_index = index + data.size();
    }
    node str(data, index);
    //超出可接收范围,直接return
    if(str._index>=_first_unacceptable() || str._end_index<_first_unassembled()) return; 
    //子串头部为已写入数据，只保留后面部分
    if(str._index<_first_unassembled()){
        str._data.erase(0, _first_unassembled()-index);
        str._index = _first_unassembled();
    }
    //尾部超出范围，删去后面部分
    if(str._end_index>_first_unacceptable()){
        str._data.erase(str._data.begin()+(_first_unacceptable()-str._index), str._data.end());
        str._end_index = _first_unacceptable();
    }
    //经处理整个字符串可以放入缓存区
    if(str._index==_first_unassembled())
        _assemble(str);                     //可以写的情况
    else
        _put_in_buff(str);                  //不能写，放入缓存区
}

void StreamReassembler::_put_in_buff(node str) {
    for(auto it=_buffer.begin(); it!=_buffer.end()&&it->_index<=str._end_index; ){
        if(it->_end_index>=str._index){
            if(it->_index<=str._index){
                if(it->_end_index<=str._end_index){
                    str._data.insert(0, it->_data, 0, str._index - it->_index);
                    str._index = it->_index;
                }
                else{
                    return;      //str完全被包含，无需任何操作
                }
            }
            else{
                if(it->_end_index>str._end_index){ //str完全包含it时,不改变str,if内为不完全包含的处理
                    str._data.append(it->_data, str._end_index - it->_index, it->_end_index - str._end_index);
                    str._end_index = it->_end_index;
                }
            }
            it = _buffer.erase(it);
        }else{
            ++it;
        }
    }
    _buffer.insert(str);
}

void StreamReassembler::_assemble(node str) {
    _output.write(str._data);
    for(auto it=_buffer.begin(); it!=_buffer.end()&&it->_index<=_first_unassembled(); ){
        if(it->_end_index>_first_unassembled()){
            _output.write(it->_data.substr(_first_unassembled() - it->_index));
        }
        it = _buffer.erase(it);
    }
    //判断是否应该结束输入
    if(_eof && _first_unassembled()==_eof_index){
        _buffer.clear();
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    int ret = 0;
    for(auto &i : _buffer){
        ret+=i._data.size();
    }
    return ret;
}

bool StreamReassembler::empty() const { return _buffer.empty(); }
