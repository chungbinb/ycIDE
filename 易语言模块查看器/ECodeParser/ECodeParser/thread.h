#pragma once

#include <Windows.h>

namespace wnxd
{
	template<typename T>
	class thread
	{
		typedef void(*FUN)(T);
	public:
		thread(FUN fun)
		{
			this->_fun = fun;
		}
		void start(T obj)
		{
			thread<T>* _this = new thread<T>(this->_fun);
			_this->_obj = obj;
			CloseHandle(CreateThread(NULL, 0, &thread<T>::_thread, _this, 0, 0));
		}
	private:
		FUN _fun;
		T _obj;
		static DWORD WINAPI _thread(void* ptr)
		{
			thread<T>* _this = (thread<T>*)ptr;
			_this->_fun(_this->_obj);
			delete _this;
			return 0;
		}
	};

	template<typename C, typename T>
	class cthread
	{
		typedef void(C::*CFUN)(T);
	public:
		cthread(C* target, CFUN fun)
		{
			this->_target = target;
			this->_cfun = fun;
		}
		void start(T obj)
		{
			cthread<C, T>* _this = new cthread<C, T>(this->_target, this->_cfun);
			_this->_obj = obj;
			CloseHandle(CreateThread(NULL, 0, &cthread<C, T>::_thread, _this, 0, 0));
		}
	private:
		C* _target;
		CFUN _cfun;
		T _obj;
		static DWORD WINAPI _thread(void* ptr)
		{
			cthread<C, T>* _this = (cthread<C, T>*)ptr;
			(_this->_target->*_this->_cfun)(_this->_obj);
			delete _this;
			return 0;
		}
	};
}