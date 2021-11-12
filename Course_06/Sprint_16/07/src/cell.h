#pragma once

#include "common.h"
#include "formula.h"

class Cell : public CellInterface {
public:
    Cell();
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

private:

    class Impl {
    public:
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;

    protected:
        Value value_;
        std::string text_;
    };

    class EmptyImpl : public Impl {
    public:
        EmptyImpl(){
            value_ = text_ = "";
        }
        Value GetValue() const override {
            return value_;
        }

        std::string GetText() const override {
            return text_;
        }
    };
    class TextImpl : public Impl {
    public:
        TextImpl(std::string_view text) {
            value_ = text_ = text;
        }

        Value GetValue() const override {
            return value_;
        }
        std::string GetText() const override {
            return text_;
        }
    };
    class FormulaImpl : public Impl {
        Value GetValue() const override {
            return value_;
        }
        std::string GetText() const override {
            return text_;
        }
    };
    std::unique_ptr<Impl> impl_;
};