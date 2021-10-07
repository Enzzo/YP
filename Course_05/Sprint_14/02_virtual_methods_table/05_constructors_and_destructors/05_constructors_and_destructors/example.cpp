//#include <iostream>
//
//using namespace std;
//
//// ----------------------------------------------------------------------------
//
//class MagicHat {
//public:
//    MagicHat() {
//    };
//
//    ~MagicHat() {
//    }
//
//    void SetVTablePtr(void* new_ptr) {
//        vtable_ptr_ = new_ptr;
//    }
//
//    void ResetVTablePtr() {
//        vtable_ptr_ = &vtable_;
//    }
//
//    // virtual method
//    void WhatInside() {
//        auto what_ptr = static_cast<VTable*>(vtable_ptr_)->what_inside;
//        (this->*what_ptr)();
//    }
//
//private:
//    void WhatInsideImpl() {
//        cout << "Nothing"s << endl;
//    }
//
//private:
//    struct VTable {
//        using WhatInsideType = void (MagicHat::*)();
//
//        WhatInsideType what_inside = { &MagicHat::WhatInsideImpl };
//    };
//
//private:
//    static VTable vtable_;
//    void* vtable_ptr_ = { &vtable_ };
//};
//
//// Init vtable
//MagicHat::VTable MagicHat::vtable_ = {};
//
//// ----------------------------------------------------------------------------
//
//class HatWithApples {
//public:
//    HatWithApples() {
//        parent_.SetVTablePtr(&vtable_);
//    };
//
//    ~HatWithApples() {
//        parent_.ResetVTablePtr();
//    }
//
//    void WhatInside() {
//        cout << "Apple"s << endl;
//    }
//
//    operator const MagicHat* () const {
//        return &parent_;
//    }
//
//    operator MagicHat() {
//        MagicHat mh;
//        mh.ResetVTablePtr();
//        return mh;
//    }
//
//private:
//    struct VTable {
//        using WhatInsideType = void (HatWithApples::*)();
//
//        WhatInsideType what_inside = { &HatWithApples::WhatInside };
//    };
//
//private:
//    static VTable vtable_;
//    MagicHat parent_;
//};
//
//HatWithApples::VTable HatWithApples::vtable_ = {};
//
//// ----------------------------------------------------------------------------
//
//int main() {
//    {
//        MagicHat* mh = (MagicHat*)(new HatWithApples);
//
//        mh->WhatInside();
//        delete mh;
//    }
//
//    {
//        MagicHat mh = HatWithApples();
//
//        mh.WhatInside();
//    }
//
//    return 0;
//}