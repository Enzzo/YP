#pragma once

class Editor {
    std::string text = {};
    std::string buffer = {};
    std::string::iterator cursor = text.begin();

public:
    Editor() = default;
    
    // сдвинуть курсор влево
    void Left() {
        if (cursor != text.begin()) {
            --cursor;
        }
    }
    
    // сдвинуть курсор вправо 
    void Right() {
        if (cursor != text.end()) {
            ++cursor;
        }
    }
    
    // вставить символ token
    void Insert(char token) {
        text.insert(cursor, token);
        std::advance(cursor, 1);
    };
    
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        buffer.erase();
        std::string::iterator begin = cursor;
        std::string::iterator end = cursor;
        
        int dist = (tokens < static_cast<size_t>(std::distance(end, text.end()))) ? tokens : std::distance(end, text.end());
        if (dist > 0) {
            std::advance(end, dist);
            buffer.insert(buffer.begin(), begin, end);
            text.erase(begin, end);
        }
    };
    
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        buffer.erase();
        std::string::iterator begin = cursor;
        std::string::iterator end = cursor;

        int dist = (tokens < static_cast<size_t>(std::distance(end, text.end()))) ? tokens : std::distance(end, text.end());
        if (dist > 0) {
            std::advance(end, dist);
            buffer.insert(buffer.begin(), begin, end);
        }
    };
    
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        text.insert(cursor, buffer.begin(), buffer.end());
        std::advance(cursor, std::distance(buffer.begin(), buffer.end()));
    };
    
    // получить текущее содержимое текстового редактора
    std::string GetText() const {
        return text;
    };
};