#pragma once

class Editor {
    std::string text_ = {};
    std::string buffer_ = {};
    std::string::iterator cursor_ = text_.begin();

public:
    Editor() = default;
    
    // �������� ������ �����
    void Left() {
        if (!IsEmpty(text_)) {
            if (cursor_ != text_.begin()) {
                --cursor_;
            }
        }
    }
    
    // �������� ������ ������ 
    void Right() {
        if (!IsEmpty(text_)) {
            if (cursor_ != text_.end()) {
                ++cursor_;
            }
        }
    }
    
    // �������� ������ token
    void Insert(char token) {
        text_.insert(cursor_, token);
        std::advance(cursor_, 1);
    };
    
    // �������� �� ����� tokens ��������, ������� � ������� ������� �������
    void Cut(size_t tokens = 1) {
        int dist = (tokens < static_cast<size_t>(std::distance(cursor_, text_.end()))) ? tokens : std::distance(cursor_, text_.end());
        buffer_.erase();

        if (dist > 0) {
            std::string::iterator begin = cursor_;
            std::string::iterator end = cursor_;
            std::advance(end, dist);
            buffer_.insert(buffer_.begin(), begin, end);
            text_.erase(begin, end);
        }
    };
    
    // c���������� �� ����� tokens ��������, ������� � ������� ������� �������
    void Copy(size_t tokens = 1) {
        int dist = (tokens < static_cast<size_t>(std::distance(cursor_, text_.end()))) ? tokens : std::distance(cursor_, text_.end());
        buffer_.erase();

        if (dist > 0) {
            std::string::iterator begin = cursor_;
            std::string::iterator end = cursor_;
            std::advance(end, dist);
            buffer_.insert(buffer_.begin(), begin, end);
        }
    };
    
    // �������� ���������� ������ � ������� ������� �������
    void Paste() {
        text_.insert(cursor_, buffer_.begin(), buffer_.end());
        std::advance(cursor_, std::distance(buffer_.begin(), buffer_.end()));
    };
    
    // �������� ������� ���������� ���������� ���������
    std::string GetText() const {
        return text_;
    };

private:
    [[nodiscard]] bool IsEmpty(const std::string& str) const noexcept{
        return str.size() == 0;
    }
};