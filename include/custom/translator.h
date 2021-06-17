#pragma once

class Translator {
    std::map<std::string_view, std::string_view> forward_;
    std::map<std::string_view, std::string_view> backward_;
    std::set<std::string> db_;

public:
    void Add(std::string_view source, std::string_view target) {
        
        std::string src(source);
        std::string trg(target);

        if (db_.count(src) == 0){
            db_.emplace(src);
        }
        if (db_.count(trg) == 0) {
            db_.emplace(trg);
        }
        
        for (const std::string& s : db_) {
            if (s == src) {
                for (const std::string& t : db_) {
                    if (t == trg) {
                        forward_.emplace(s, t);
                        backward_.emplace(t, s);
                        forward_.emplace(t, "");
                        backward_.emplace(s, "");
                        break;
                    }
                }
                break;
            }
        }        
    }
    std::string_view TranslateForward(std::string_view source) const {
        return forward_.at(source);
    }
    std::string_view TranslateBackward(std::string_view target) const {
        return backward_.at(target);
    }
};