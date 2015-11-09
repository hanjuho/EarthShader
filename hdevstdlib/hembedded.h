#pragma once
#pragma warning(disable : 4251)



#include "hdefbase.h"
#include <map>
#include <vector>
#include <string>



#define EMBEDDED_TYPE typename _hardware, typename _software, typename _key = std::wstring
#define DEL_SW(p) IF_NOT_NULL(p){p->release(); p = NULL;}



namespace h
{
	/*
	이 패턴은 리소스 관리를 위해 만들어졌다.
	원본 데이터를 하드웨어로 표현했고, 접근자를 소프트웨어로 표현했다.
	하드웨어 따라 소프트웨어가 달라지는 그 모습이 임베디드 시스템과 닮아 이 패턴의 이름을 임베디드 아키텍처 패턴이라 지었다.
	*/
	TEMPLATE_INTERFACE(EMBEDDED_TYPE, Embedded_Architecture_Pattern)
	{
		/*
		$ HW = data가 함수를 갖는 것은 좋지 않을 수도 있다.
		2. HW = data의 변수 생성은 _factory에서 결정된다.
		3. HW = data의 변수 소멸은 _factory에서 결정된다.
		*/
		typedef _hardware	HW, &ref_HW, *ptr_HW;

		typedef	_key		KEY;

		/*
		$ _software = accessor의 파생 클래스, _software = accessor는 HW = data에 접근하는 수단을 제공해야 한다.
		1. HW = data가 소멸되면 SW::invaild가 호출된다.
		2. SW 타입 포인터 변수를 초기화하면 반드시 addRef을 호출한다.
		3. SW 타입 포인터 변수가 소멸되기 전 반드시 release를 호출한다.
		4. SW::release가 호출되어 참조값이 0이 되면 객체가 소멸된다.
		*/
		DECL_INHERITANCE(CLASS, SW, _software)
		{
		public:
			CLASS_DECL_CONSTRUCTOR(SW)(DWORD id)
				: my_ref(0), my_id(id)
			{
			}

			CLASS_DECL_FUNC_T(DWORD, get_id)(VOID)CONST
			{
				return my_id;
			}

			CLASS_DECL_FUNC(addRef)(VOID)
			{
				return InterlockedIncrement(&my_ref);
			}

			CLASS_DECL_FUNC(release)(VOID)
			{
				LONG ref = InterlockedDecrement(&my_ref);
				IF_TRUE(ref < 0)
				{
					delete this;
				}

				return ref;
			}

			INTERFACE_DECL_FUNC(invalid)(VOID)PURE;

		protected:
			CLASS_DECL_DESTRUCTOR(SW)(VOID)
			{
			}

		private:
			LONG	my_ref;
			DWORD	my_id;
		} *ptr_SW;

		/*
		$ Package_Bundle의 value에 해당한다.
		1. Package의 id는 _factory에서 결정된다.
		*/
		DECL_STRUCT(Package)
		{
			CLASS_DECL_CONSTRUCTOR(Package)(VOID)
				: id(0xFFFFFFFF), sw(NULL)
			{
			}
			DWORD	id;
			HW		hw;
			ptr_SW	sw;
		} *ptr_Package, &ref_Package;

		/*
		$ 기본 클래스인 Container에 의해서 Package가 추가될 때 Package가 저장되는 공간.
		1. 자료구조는 multimap이다.
		*/
		typedef std::multimap<KEY, Package>		Package_Bundle;
		typedef std::multimap<KEY, Package> *	ptr_Package_Bundle;

		/*
		$ 기본 클래스인 Container에 의해서 Package가 추가될 때 SW가 저장되는 공간.
		1. stl tree에서 도중에 사라지는 원소의 애매한 처리를 방지하기 위해 만들어졌다.
		*/
		typedef std::vector<ptr_SW>		SW_Buffer;
		typedef std::vector<ptr_SW>	*	ptr_SW_Buffer;

		/*
		$ _hw, SW를 결정한다.
		1. develop_sw에서 ptr_SW * pSw의 파라미터에 외부에서 생성된 변수를 넣는 것은 위험하다.
		*/
		DECL_INTERFACE(Factory)
		{
			INTERFACE_DECL_FUNC(construct_hw)		(CONST KEY key, CONST DWORD option, DWORD * id, ptr_HW hw)CONST PURE;
			INTERFACE_DECL_FUNC(destruct_hw)		(CONST KEY key, CONST DWORD id, ptr_HW hw)CONST PURE;
			INTERFACE_DECL_FUNC(develop_sw)			(CONST ptr_HW hw, CONST DWORD id, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC_T(DWORD, create_id)	(CONST KEY key, CONST DWORD option)CONST PURE;
			INTERFACE_DECL_FUNC_T(DWORD, get_type)	(VOID)CONST PURE;
		} *ptr_Factory;

		/*
		$ Embedded_Architecture_Pattern에서 Package_Bundle를 수정할 수 있는 유일한 수단을 제공한다.
		1. ptr_SW * pSw 파라미터에 값이 들어가면 SW::addRef가 호출될 것을 권장한다.
		2. 1번이 충족된 경우 사용이 끝난 pSw는 반드시 SW::release를 호출해야 한다.
		*/
		DECL_INTERFACE(Container)
		{
			INTERFACE_DECL_FUNC(push)		(CONST KEY key, CONST DWORD option, ptr_SW * pSw)PURE;
			INTERFACE_DECL_FUNC(pop)		(CONST KEY key, CONST DWORD id)PURE;
			INTERFACE_DECL_FUNC(pop_all)	(CONST KEY key)PURE;
			INTERFACE_DECL_FUNC(move)		(CONST KEY from, CONST DWORD id, CONST KEY to, ptr_SW * pSw)PURE;
			INTERFACE_DECL_FUNC(move_all)	(CONST KEY from, CONST KEY to, LONG count, ptr_SW * pSw)PURE;
			INTERFACE_DECL_FUNC(sort)		(VOID)PURE;
			INTERFACE_DECL_FUNC_T(DWORD, stock)		(KEY CONST key)PURE;
			INTERFACE_DECL_FUNC_T(DWORD, stock_all)	(VOID)PURE;
		} *ptr_Container;

		/*
		$ Embedded_Architecture_Pattern에서 Package_Bundle를 검색할 수 있는 유일한 수단을 제공한다.
		*/
		DECL_INTERFACE(Finder)
		{
			INTERFACE_DECL_FUNC(find_explicit)		(CONST KEY key, CONST DWORD id, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC(find_explicit_all)	(CONST KEY key, LONG count, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC(find_implicit)		(CONST KEY key, CONST DWORD format, CONST LPVOID data, LONG count, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC(find_implicit_all)	(CONST DWORD format, CONST LPVOID data, LONG count, ptr_SW * pSw)CONST PURE;
		} *ptr_Finder;

		/*
		$ Embedded_Architecture_Pattern에서 Package_Bundle를 순회할 수 있는 유일한 수단을 제공한다.
		1. 생성자 파라미터에 null을 넣으면 예외가 발생한다.
		2. access를 호출하기 전 반드시 next를 호출한다.
		3. access를 호출할 때 SW::addRef은 호출되지 않는다.
		*/
		DECL_STRUCT(iterator)
		{
			CLASS_DECL_CONSTRUCTOR(iterator)(CONST ptr_SW_Buffer buffer)
				: my_buffer(buffer), my_index(0xFFFFFFFF), my_sw(NULL)
			{
				IF_NULL(buffer)
				{
					throw;
				}
			}

			CLASS_DECL_DESTRUCTOR(iterator)(VOID)
			{
				DEL_SW(my_sw);
			}

			CLASS_DECL_FUNC(next)(VOID)
			{
				LONG index = my_index + 1;
				IF_FALSE(index < LONG(my_buffer->size()))
				{
					return E_FAIL;
				}

				DEL_SW(my_sw);
				my_sw = (*my_buffer)[index];
				my_sw->addRef();

				my_index = index;

				return my_index;
			}

			CLASS_DECL_FUNC_T(ptr_SW, access)(VOID)CONST
			{
				return my_sw;
			}

			VOID operator = (ptr_HW & iter)
			{
				my_buffer = iter.my_buffer;
				my_buffer = iter.my_index;
				my_sw = iter->my_sw;
				IF_NOT_NULL(my_sw)
				{
					my_sw->addRef();
				}
			}

			bool operator == (CONST ptr_HW & iter) CONST
			{
				IF_TRUE(my_sw == iter.my_sw)
				{
					return true;
				}

				return false;
			}

		private:
			CONST ptr_SW_Buffer my_buffer;
			LONG	my_index;
			ptr_SW	my_sw;
		} *ptr_iterator;

		/*
		$ Embedded_Architecture_Pattern에서 _hw, SW를 위해 기본으로 제공하는 저장 객체
		1. 이 객체는 Container의 파라미터가 된다.
		2. 이 객체가 스스로 할 수 있는 일은 없다.
		*/
		DECL_STRUCT(Pattern_Buffer)
		{
			Package_Bundle	bundle;
			SW_Buffer 		buffer;
		} *ptr_Pattern_Buffer;

		/*
		$ _container의 파생 클래스, Embedded_Architecture_Pattern이 제공하는 기본 _container 클래스
		1. 생성자 파라미터의 값에 null이 있으면 예외를 호출한다.
		*/
		DECL_INHERITANCE(CLASS, Pattern_Container, Container)
		{
		public:
			CLASS_DECL_CONSTRUCTOR(Pattern_Container)(ptr_Factory factory, ptr_Pattern_Buffer buffer)
				: my_factory(factory), my_buffer(buffer)
			{
				IF_NULL((factory && buffer))
				{
					throw;
				}
			}

			CLASS_DECL_DESTRUCTOR(Pattern_Container)(VOID)
			{
			}

			CLASS_DECL_FUNC(setFactory)(ptr_Factory factory)
			{
				IF_NULL(factory)
				{
					return E_INVALIDARG;
				}

				my_factory = factory;

				return S_OK;
			}

			CLASS_DECL_FUNC(setPatternBuffer)(ptr_Pattern_Buffer buffer)
			{
				IF_NULL(buffer)
				{
					return E_INVALIDARG;
				}

				my_buffer = buffer;

				return S_OK;
			}

			INTERFACE_DECL_FUNC(push)(CONST KEY key, CONST DWORD option, ptr_SW * pSw)
			{
				// _hw, SW를 포함하는 구조체의 이터레이터
				Package_Bundle::iterator iter;

				try
				{
					// Package_Bundle, package를 key 부류에 추가
					iter = my_buffer->bundle.insert(Package_Bundle::value_type(key, Package()));
				}
				catch (...)
				{
					return E_OUTOFMEMORY;
				}

				ref_Package package = iter->second;

				// 반환 값
				HRESULT hr;

				// _factory로부터 key과 option에 따른 id와 hw에 대한 정보를 받아온다.
				IF_FAILED(hr = my_factory->construct_hw(key, option, &package.id, &package.hw))
				{
					return hr;
				}

				// _factory로부터 hw와 id에 따른 SW에 대한 정보를 받아온다.
				IF_FAILED(hr = my_factory->develop_sw(&package.hw, package.id, &package.sw))
				{
					// 실패할 경우 hw에 대한 정보를 반납한다.
					my_factory->destruct_hw(key, package.id, &package.hw);

					// 실패할 경우 이터레이터를 항목에서 지운다.
					my_buffer->bundle.erase(iter);

					return hr;
				}

				try
				{
					// SW_Buffer, iterator를 위한 SW 버퍼에 SW 추가
					my_buffer->buffer.push_back(package.sw);
				}
				catch (...)
				{
					return E_OUTOFMEMORY;
				}

				// 사용자가 SW 정보를 원하는 경우 반환
				IF_NOT_NULL(pSw)
				{
					package.sw->addRef();
					(*pSw) = package.sw;
				}

				return S_OK;
			}

			INTERFACE_DECL_FUNC(pop)(CONST KEY key, CONST DWORD id)
			{
				// key 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// 걸러진 Package에 대해서 id를 검사한다.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// Package를 찾았다면 
					IF_TRUE(iter->second.id == id)
					{
						// sw를 무효화하고
						iter->second.sw->invalid();
						// SW의 참조카운트를 감소시킨다.
						iter->second.sw->release();

						// id와 _hw 정보를 반납힌다.
						my_factory->destruct_hw(key, id, &iter->second.hw);

						// Package_Bundle에서 Package를 제거한다.
						my_buffer->bundle.erase(iter);

						return S_OK;
					}
				}

				return E_INVALIDARG;
			}

			INTERFACE_DECL_FUNC(pop_all)(CONST KEY key)
			{
				// key 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// 걸러진 Package를 탐색한다.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// SW를 무효화하고
					iter->second.sw->invalid();
					// SW의 참조카운트를 감소시킨다.
					iter->second.sw->release();

					// id와 _hw 정보를 반납힌다.
					my_factory->destruct_hw(key, iter->second.id, &iter->second.hw);
				}

				// Package_Bundle에서 key 부류의 Package를 모두 제거한다.
				my_buffer->bundle.erase(list.first, list.second);

				return S_OK;
			}

			INTERFACE_DECL_FUNC(pop_all)(VOID)
			{
				// Package_Bundle의 모든 Package를 제거한다.
				Package_Bundle::iterator begin = my_buffer->bundle.begin();
				Package_Bundle::iterator end = my_buffer->bundle.end();
				for (Package_Bundle::iterator iter = begin; iter != end; ++iter)
				{
					// SW를 무효화하고
					iter->second.sw->invalid();
					// SW의 참조카운트를 감소시킨다.
					iter->second.sw->release();

					// id와 _hw 정보를 반납힌다.
					my_factory->destruct_hw(iter->first, iter->second.id, &iter->second.hw);
				}

				my_buffer->bundle.clear();

				return S_OK;
			}

			INTERFACE_DECL_FUNC(move)(CONST KEY from, CONST DWORD id, CONST KEY to, ptr_SW * pSw)
			{
				// from 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(from);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				IF_NOT_NULL(pSw)
				{
					(*pSw) = NULL;
				}

				// 걸러진 Package를 탐색한다.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// Package를 찾았다면 
					IF_TRUE(iter->second.id == id)
					{
						// iterator
						Package_Bundle::iterator package;

						// to 부류에 원소를 추가하고
						package = my_buffer->bundle.insert(Package_Bundle::value_type(to, iter->second));

						IF_NOT_NULL(pSw)
						{
							package->second.sw->addRef();
							(*pSw) = package->second.sw;
						}

						// from 부류에서 원소를 제거한다.
						my_buffer->bundle.erase(iter);

						return S_OK;
					}
				}

				return E_INVALIDARG;
			}

			INTERFACE_DECL_FUNC(move_all)(CONST KEY from, CONST KEY to, LONG count, ptr_SW * pSw)
			{
				// from 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(from);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				IF_TRUE(list.first == list.second)
				{
					return E_INVALIDARG;
				}

				// index
				LONG index = 0;

				IF_NULL(pSw)
				{
					// 걸러진 Package를 탐색한다.
					for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
					{
						// to 부류에 원소를 추가하고
						my_buffer->bundle.insert(Package_Bundle::value_type(to, iter->second));

						++index;
					}
				}
				else
				{
					IF_NOT_NULL(pSw)
					{
						// 버퍼 초기화
						ZeroMemory(pSw, sizeof(LONG)* count);
					}

					// iterator
					Package_Bundle::iterator SW;

					// 걸러진 Package를 탐색한다.
					for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
					{
						// to 부류에 원소를 추가하고
						SW = my_buffer->bundle.insert(Package_Bundle::value_type(to, iter->second));

						IF_TRUE(index < count)
						{
							SW->second.sw->addRef();
							pSw[index] = SW->second.sw;
						}

						++index;
					}
				}

				// Package_Bundle에서 from 부류의 Package를 모두 제거한다.
				my_buffer->bundle.erase(list.first, list.second);

				return index;
			}

			INTERFACE_DECL_FUNC_T(DWORD, stock)(CONST KEY key)
			{
				// from 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// stock
				LONG stock = 0;

				// key 부류의 Package 개수를 탐색한다.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					stock++;
				}

				return stock;
			}

			INTERFACE_DECL_FUNC_T(DWORD, stock_all)(VOID)
			{
				return my_buffer->bundle.size();
			}

			INTERFACE_DECL_FUNC(sort)(VOID)
			{
				// SW_Buffer
				SW_Buffer & buffer = my_buffer->buffer;

				// SW_Buffer의 원소를 모두 제거한다.
				for (LONG index = buffer.size(); index != -1; --index)
				{
					buffer[index]->release();
				}

				// Package_Bundle의 크기만큼 SW_Buffer의 크기를 다시 설정한다.
				buffer.resize(stock_all());

				// index
				DWORD index = 0;

				// Package_Bundle의 원소인 Package에서 SW만을 저장하고 SW::addRef()를 호출한다.
				Package_Bundle::iterator begin = my_buffer->bundle.begin();
				Package_Bundle::iterator end = my_buffer->bundle.end();
				for (Package_Bundle::iterator iter = begin; iter != end; ++iter)
				{
					buffer[index] = iter->second.sw;

					buffer[index]->addRef();

					++index;
				}

				return S_OK;
			}

		protected:
			ptr_Factory			my_factory;
			ptr_Pattern_Buffer	my_buffer;
		} &ref_Container;

		/*
		$ _finder의 파생 클래스, Embedded_Architecture_Pattern이 제공하는 기본 _finder 클래스
		1. 생성자 파라미터의 값에 null이 있으면 예외를 호출한다.
		2. implicit에 관련된 함수는 상속을 받아 따로 만들어야 한다.
		*/
		DECL_INHERITANCE(CLASS, Pattern_Finder, Finder)
		{
		public:
			CLASS_DECL_CONSTRUCTOR(Pattern_Finder)(ptr_Pattern_Buffer buffer)
				: my_buffer(buffer)
			{
				IF_NULL(buffer)
				{
					throw;
				}
			}

			CLASS_DECL_DESTRUCTOR(Pattern_Finder)(VOID)
			{
			}

			CLASS_DECL_FUNC(setPatternBuffer)(ptr_Pattern_Buffer buffer)
			{
				IF_NULL(buffer)
				{
					return E_INVALIDARG;
				}

				my_buffer = buffer;

				return S_OK;
			}

			INTERFACE_DECL_FUNC(find_explicit)(CONST KEY key, CONST DWORD id, ptr_SW * pSw)CONST
			{
				IF_NULL(pSw)
				{
					return E_INVALIDARG;
				}

				// 반환 값 초기화
				(*pSw) = NULL;

				// key 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// 걸러진 Package에 대해서 id를 검사한다.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// Package를 찾았다면 
					IF_TRUE(iter->second.id == id)
					{
						iter->second.sw->addRef();
						(*pSw) = iter->second.sw;

						return S_OK;
					}
				}

				return E_INVALIDARG;
			}

			INTERFACE_DECL_FUNC(find_explicit_all)(CONST KEY key, LONG count, ptr_SW * pSw)CONST
			{
				IF_NULL(pSw)
				{
					return E_INVALIDARG;
				}
				else
				{
					// 버퍼 초기화
					ZeroMemory(pSw, sizeof(LONG)* count);
				}

				// key 부류의 모든 Package를 불러온다.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// index
				LONG index = 0;

				// 걸러진 Package에 대해서 id를 검사한다.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					IF_TRUE(index < count)
					{
						iter->second.sw->addRef();
						pSw[index] = iter->second.sw;
					}

					++index;
				}

				return index;
			}

			INTERFACE_DECL_FUNC(find_implicit)(CONST KEY key, CONST DWORD format, CONST LPVOID data, LONG count, ptr_SW * pSw)CONST
			{
				return E_FAIL;
			}

			INTERFACE_DECL_FUNC(find_implicit_all)(CONST DWORD format, CONST LPVOID data, LONG count, ptr_SW * pSw)CONST
			{
				return E_FAIL;
			}

		protected:
			ptr_Pattern_Buffer my_buffer;
		} &ref_Finder;


		INTERFACE_DECL_FUNC_T(ptr_Factory, get_Factory)		(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(ptr_Container, get_Container)	(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(ptr_Finder, get_Finder)		(VOID)CONST PURE;

		/*
		$ 순회자를 동적 생성하는 비용이 비싸고, 순회 방법이 달라 순회자를 파생 클래스가 정의하게 했다.
		*/
	};
};