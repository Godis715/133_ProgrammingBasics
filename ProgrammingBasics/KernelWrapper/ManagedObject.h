//#pragma once
//
//using namespace System;
//
//namespace KernelWrapper {
//	template<class T>
//	public ref class ManagedObject
//	{
//	protected:
//		T * instance;
//	public:
//		ManagedObject(T* _instance) : instance(_instance) {
//			instance = _instance;
//		}
//		virtual ~ManagedObject() {
//			if (instance != nullptr) {
//				delete instance;
//			}
//		}
//		!ManagedObject() {
//			if (instance != nullptr) {
//				delete instance;
//			}
//		}
//		T* GetInstance() {
//			return instance;
//		}
//	};
//}
