//#include <iostream>
//#include <string>
//#include <limits>
//#include <array>
//
//using namespace std::literals;
//
//class Base {
//private:
//	struct VTable;
//
//public:
//	void Foo1() const {
//		auto foo1_impl = static_cast<VTable*>(vtable_ptr_)->vfoo;
//		(this->*foo1_impl)();
//	}
//
//	int Foo2(const int arg) const {
//		auto foo2_impl = static_cast<VTable*>(vtable_ptr_)->ifoo;
//		return (this->*foo2_impl)(arg);
//	}
//
//	void SetVtablePtr(void* deriv) {
//		vtable_ptr_ = deriv;
//	}
//
//	void FreeVtablePtr() {
//		vtable_ptr_ = &vtable_;
//	}
//
//private:
//	void Foo1Impl()const {
//		std::cout << "base"sv << std::endl;
//	}
//
//	int Foo2Impl(const int arg)const {
//		return arg;
//	}
//	struct VTable {
//		using VoidFooType = void(Base::*)()const;
//		using IntFooType = int(Base::*)(const int) const;
//
//		VoidFooType vfoo;
//		IntFooType ifoo;
//	};
//
//private:
//	static VTable vtable_;
//	void* vtable_ptr_ = &vtable_;
//};
//
//Base::VTable Base::vtable_ = {
//	&Base::Foo1Impl,
//	&Base::Foo2Impl
//};
//
//class Deriv {
//public:
//
//	Deriv() {
//		parent_.SetVtablePtr(&vtable_);
//	}
//
//	~Deriv() {
//		parent_.FreeVtablePtr();
//	}
//
//	//для наследников
//	void SetVtablePtr(void* deriv) {
//		vtable_ptr_ = deriv;
//	}
//
//	//для наследников
//	void FreeVtablePtr() {
//		vtable_ptr_ = &vtable_;
//	}
//
//	//virtual from base
//	void Foo1()const {
//		auto foo1_impl = static_cast<VTable*>(vtable_ptr_)->vfoo;
//		return (this->*foo1_impl)();
//	}
//
//	//virtual from base
//	int Foo2(const int arg)const {
//		auto foo2_impl = static_cast<VTable*>(vtable_ptr_)->ifoo;
//		return (this->*foo2_impl)(arg);
//	}
//
//	void Foo3()const {
//		auto foo3_impl = static_cast<VTable*>(vtable_ptr_)->v3foo;
//		return (this->*foo3_impl)();
//	}
//
//private:
//	void Foo1Impl()const {
//		std::cout << "deriv"sv << std::endl;
//	}
//	int Foo2Impl(const int arg)const {
//		return arg * arg;
//	}
//	void Foo3Impl()const {
//		std::cout << "derived own" << std::endl;
//	}
//
//	struct VTable {
//		using VoidFooType = void(Deriv::*)()const;
//		using IntFooType = int(Deriv::*)(const int) const;
//
//		VoidFooType vfoo;
//		IntFooType ifoo;
//		VoidFooType v3foo;
//	};
//
//private:
//	static VTable vtable_;
//	void* vtable_ptr_ = &vtable_;
//	Base parent_;
//};
//
//Deriv::VTable Deriv::vtable_ = {
//	&Deriv::Foo1Impl,
//	&Deriv::Foo2Impl
//};
//
//class SubDeriv {
//public:
//	SubDeriv() {
//		parent_.SetVtablePtr(&vtable_);
//	}
//	~SubDeriv() {
//		parent_.FreeVtablePtr();
//	}
//
//	void Foo1()const {
//		auto foo_impl = static_cast<VTable*>(vtable_ptr_)->foo1;
//		return (this->*foo_impl)();
//	}
//
//	int Foo2(const int arg) const {
//		auto foo2_impl = static_cast<VTable*>(vtable_ptr_)->foo2;
//		return (this->*foo2_impl)(arg);
//	}
//	void Foo3()const {
//		auto foo_impl = static_cast<VTable*>(vtable_ptr_)->foo3;
//		return (this->*foo_impl)();
//	}
//
//private:
//	void Foo1Impl()const {
//		std::cout << "sub"sv << std::endl;
//	}
//	int Foo2Impl(const int arg)const {
//		return arg * arg * arg;
//	}
//	void Foo3Impl()const {
//		std::cout << "sub own" << std::endl;
//	}
//private:
//	struct VTable {
//		using VoidFooType = void(SubDeriv::*)()const;
//		using IntFooType = int(SubDeriv::*)(const int) const;
//		VoidFooType foo1;
//		IntFooType foo2;
//		VoidFooType foo3;
//	};
//
//private:
//	static VTable vtable_;
//	void* vtable_ptr_ = &vtable_;
//	Deriv parent_;
//};
//
//SubDeriv::VTable SubDeriv::vtable_ = {
//	&SubDeriv::Foo1Impl,
//	&SubDeriv::Foo2Impl,
//	&SubDeriv::Foo3Impl
//};
//
//int main() {
//	Base b;
//	b.Foo1();
//	std::cout << b.Foo2(3) << std::endl;
//
//	Base* c = (Base*)(new Deriv);
//	c->Foo1();
//	std::cout << c->Foo2(3) << std::endl;
//	delete c;
//
//	Deriv* d = (Deriv*)(new SubDeriv);
//	d->Foo1();
//	std::cout << d->Foo2(3) << std::endl;
//	d->Foo3();
//	delete d;
//
//	return 0;
//}