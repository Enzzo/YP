#pragma once

class Editor {
    std::list<char> text_ = {};
    std::list<char> buffer_ = {};
    size_t cursor_ = 0;
    
public:
    Editor() = default;
    
    // сдвинуть курсор влево
    void Left() {
        if (cursor_ > 0) {
            --cursor_;
        }
    }
    
    // сдвинуть курсор вправо 
    void Right() {
        if (cursor_ < text_.size()) {
            ++cursor_;
        }
    }
    
    // вставить символ token
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
    
    // вырезать не более tokens символов, начиная с текущей позиции курсора
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
    
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
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
    
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        if (!buffer_.empty()) {
            std::list<char>::iterator pos = text_.begin();
            std::advance(pos, cursor_);

            text_.insert(pos, buffer_.begin(), buffer_.end());
            cursor_ += buffer_.size();
        }
    };
    
    // получить текущее содержимое текстового редактора
    std::string GetText() const {
        return { text_.begin(), text_.end() };
    };
};