#pragma once

class Editor {
    std::list<char> text_ = {};
    std::list<char> buffer_ = {};
    size_t cursor_ = 0;
    
public:
    Editor() = default;
    
    // �������� ������ �����
    void Left() {
        if (cursor_ > 0) {
            --cursor_;
        }
    }
    
    // �������� ������ ������ 
    void Right() {
        if (cursor_ < text_.size()) {
            ++cursor_;
        }
    }
    
    // �������� ������ token
    void Insert(char token) {
        if (text_.empty()) {
            text_.push_back(token);
        }
        else {
            std::list<char>::iterator it = text_.begin();
            std::advance(it, cursor_);
            text_.emplace(it, token);
        }
        cursor_++;
    };
    
    // �������� �� ����� tokens ��������, ������� � ������� ������� �������
    void Cut(size_t tokens = 1) {
        if (!buffer_.empty()) {
            buffer_.erase(buffer_.begin(), buffer_.end());
        }

        if (tokens > 0) {
            std::list<char>::iterator begin = text_.begin();
            std::advance(begin, cursor_);

            int dist = (tokens < static_cast<size_t>(std::distance(begin, text_.end()))) ? tokens : std::distance(begin, text_.end());
            
            if (dist > 0) {
                std::list<char>::iterator end = begin;
                std::advance(end, dist);
                buffer_.insert(buffer_.end(), begin, end);
                text_.erase(begin, end);
            }
        }
    };
    
    // c���������� �� ����� tokens ��������, ������� � ������� ������� �������
    void Copy(size_t tokens = 1) {
        if (!buffer_.empty()) {
            buffer_.erase(buffer_.begin(), buffer_.end());
        }

        if (tokens > 0) {
            std::list<char>::iterator begin = text_.begin();
            std::advance(begin, cursor_);

            int dist = (tokens < static_cast<size_t>(std::distance(begin, text_.end()))) ? tokens : std::distance(begin, text_.end());

            if (dist > 0) {
                std::list<char>::iterator end = begin;
                std::advance(end, dist);
                buffer_.insert(buffer_.end(), begin, end);
            }
        }
    };
    
    // �������� ���������� ������ � ������� ������� �������
    void Paste() {
        if (!buffer_.empty()) {
            std::list<char>::iterator pos = text_.begin();
            std::advance(pos, cursor_);

            text_.insert(pos, buffer_.begin(), buffer_.end());
            cursor_ += buffer_.size();
        }
    };
    
    // �������� ������� ���������� ���������� ���������
    std::string GetText() const {
        return { text_.begin(), text_.end() };
    };
};