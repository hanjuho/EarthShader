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
	�� ������ ���ҽ� ������ ���� ���������.
	���� �����͸� �ϵ����� ǥ���߰�, �����ڸ� ����Ʈ����� ǥ���ߴ�.
	�ϵ���� ���� ����Ʈ��� �޶����� �� ����� �Ӻ���� �ý��۰� ��� �� ������ �̸��� �Ӻ���� ��Ű��ó �����̶� ������.
	*/
	TEMPLATE_INTERFACE(EMBEDDED_TYPE, Embedded_Architecture_Pattern)
	{
		/*
		$ HW = data�� �Լ��� ���� ���� ���� ���� ���� �ִ�.
		2. HW = data�� ���� ������ _factory���� �����ȴ�.
		3. HW = data�� ���� �Ҹ��� _factory���� �����ȴ�.
		*/
		typedef _hardware	HW, &ref_HW, *ptr_HW;

		typedef	_key		KEY;

		/*
		$ _software = accessor�� �Ļ� Ŭ����, _software = accessor�� HW = data�� �����ϴ� ������ �����ؾ� �Ѵ�.
		1. HW = data�� �Ҹ�Ǹ� SW::invaild�� ȣ��ȴ�.
		2. SW Ÿ�� ������ ������ �ʱ�ȭ�ϸ� �ݵ�� addRef�� ȣ���Ѵ�.
		3. SW Ÿ�� ������ ������ �Ҹ�Ǳ� �� �ݵ�� release�� ȣ���Ѵ�.
		4. SW::release�� ȣ��Ǿ� �������� 0�� �Ǹ� ��ü�� �Ҹ�ȴ�.
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
		$ Package_Bundle�� value�� �ش��Ѵ�.
		1. Package�� id�� _factory���� �����ȴ�.
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
		$ �⺻ Ŭ������ Container�� ���ؼ� Package�� �߰��� �� Package�� ����Ǵ� ����.
		1. �ڷᱸ���� multimap�̴�.
		*/
		typedef std::multimap<KEY, Package>		Package_Bundle;
		typedef std::multimap<KEY, Package> *	ptr_Package_Bundle;

		/*
		$ �⺻ Ŭ������ Container�� ���ؼ� Package�� �߰��� �� SW�� ����Ǵ� ����.
		1. stl tree���� ���߿� ������� ������ �ָ��� ó���� �����ϱ� ���� ���������.
		*/
		typedef std::vector<ptr_SW>		SW_Buffer;
		typedef std::vector<ptr_SW>	*	ptr_SW_Buffer;

		/*
		$ _hw, SW�� �����Ѵ�.
		1. develop_sw���� ptr_SW * pSw�� �Ķ���Ϳ� �ܺο��� ������ ������ �ִ� ���� �����ϴ�.
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
		$ Embedded_Architecture_Pattern���� Package_Bundle�� ������ �� �ִ� ������ ������ �����Ѵ�.
		1. ptr_SW * pSw �Ķ���Ϳ� ���� ���� SW::addRef�� ȣ��� ���� �����Ѵ�.
		2. 1���� ������ ��� ����� ���� pSw�� �ݵ�� SW::release�� ȣ���ؾ� �Ѵ�.
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
		$ Embedded_Architecture_Pattern���� Package_Bundle�� �˻��� �� �ִ� ������ ������ �����Ѵ�.
		*/
		DECL_INTERFACE(Finder)
		{
			INTERFACE_DECL_FUNC(find_explicit)		(CONST KEY key, CONST DWORD id, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC(find_explicit_all)	(CONST KEY key, LONG count, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC(find_implicit)		(CONST KEY key, CONST DWORD format, CONST LPVOID data, LONG count, ptr_SW * pSw)CONST PURE;
			INTERFACE_DECL_FUNC(find_implicit_all)	(CONST DWORD format, CONST LPVOID data, LONG count, ptr_SW * pSw)CONST PURE;
		} *ptr_Finder;

		/*
		$ Embedded_Architecture_Pattern���� Package_Bundle�� ��ȸ�� �� �ִ� ������ ������ �����Ѵ�.
		1. ������ �Ķ���Ϳ� null�� ������ ���ܰ� �߻��Ѵ�.
		2. access�� ȣ���ϱ� �� �ݵ�� next�� ȣ���Ѵ�.
		3. access�� ȣ���� �� SW::addRef�� ȣ����� �ʴ´�.
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
		$ Embedded_Architecture_Pattern���� _hw, SW�� ���� �⺻���� �����ϴ� ���� ��ü
		1. �� ��ü�� Container�� �Ķ���Ͱ� �ȴ�.
		2. �� ��ü�� ������ �� �� �ִ� ���� ����.
		*/
		DECL_STRUCT(Pattern_Buffer)
		{
			Package_Bundle	bundle;
			SW_Buffer 		buffer;
		} *ptr_Pattern_Buffer;

		/*
		$ _container�� �Ļ� Ŭ����, Embedded_Architecture_Pattern�� �����ϴ� �⺻ _container Ŭ����
		1. ������ �Ķ������ ���� null�� ������ ���ܸ� ȣ���Ѵ�.
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
				// _hw, SW�� �����ϴ� ����ü�� ���ͷ�����
				Package_Bundle::iterator iter;

				try
				{
					// Package_Bundle, package�� key �η��� �߰�
					iter = my_buffer->bundle.insert(Package_Bundle::value_type(key, Package()));
				}
				catch (...)
				{
					return E_OUTOFMEMORY;
				}

				ref_Package package = iter->second;

				// ��ȯ ��
				HRESULT hr;

				// _factory�κ��� key�� option�� ���� id�� hw�� ���� ������ �޾ƿ´�.
				IF_FAILED(hr = my_factory->construct_hw(key, option, &package.id, &package.hw))
				{
					return hr;
				}

				// _factory�κ��� hw�� id�� ���� SW�� ���� ������ �޾ƿ´�.
				IF_FAILED(hr = my_factory->develop_sw(&package.hw, package.id, &package.sw))
				{
					// ������ ��� hw�� ���� ������ �ݳ��Ѵ�.
					my_factory->destruct_hw(key, package.id, &package.hw);

					// ������ ��� ���ͷ����͸� �׸񿡼� �����.
					my_buffer->bundle.erase(iter);

					return hr;
				}

				try
				{
					// SW_Buffer, iterator�� ���� SW ���ۿ� SW �߰�
					my_buffer->buffer.push_back(package.sw);
				}
				catch (...)
				{
					return E_OUTOFMEMORY;
				}

				// ����ڰ� SW ������ ���ϴ� ��� ��ȯ
				IF_NOT_NULL(pSw)
				{
					package.sw->addRef();
					(*pSw) = package.sw;
				}

				return S_OK;
			}

			INTERFACE_DECL_FUNC(pop)(CONST KEY key, CONST DWORD id)
			{
				// key �η��� ��� Package�� �ҷ��´�.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// �ɷ��� Package�� ���ؼ� id�� �˻��Ѵ�.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// Package�� ã�Ҵٸ� 
					IF_TRUE(iter->second.id == id)
					{
						// sw�� ��ȿȭ�ϰ�
						iter->second.sw->invalid();
						// SW�� ����ī��Ʈ�� ���ҽ�Ų��.
						iter->second.sw->release();

						// id�� _hw ������ �ݳ�����.
						my_factory->destruct_hw(key, id, &iter->second.hw);

						// Package_Bundle���� Package�� �����Ѵ�.
						my_buffer->bundle.erase(iter);

						return S_OK;
					}
				}

				return E_INVALIDARG;
			}

			INTERFACE_DECL_FUNC(pop_all)(CONST KEY key)
			{
				// key �η��� ��� Package�� �ҷ��´�.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// �ɷ��� Package�� Ž���Ѵ�.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// SW�� ��ȿȭ�ϰ�
					iter->second.sw->invalid();
					// SW�� ����ī��Ʈ�� ���ҽ�Ų��.
					iter->second.sw->release();

					// id�� _hw ������ �ݳ�����.
					my_factory->destruct_hw(key, iter->second.id, &iter->second.hw);
				}

				// Package_Bundle���� key �η��� Package�� ��� �����Ѵ�.
				my_buffer->bundle.erase(list.first, list.second);

				return S_OK;
			}

			INTERFACE_DECL_FUNC(pop_all)(VOID)
			{
				// Package_Bundle�� ��� Package�� �����Ѵ�.
				Package_Bundle::iterator begin = my_buffer->bundle.begin();
				Package_Bundle::iterator end = my_buffer->bundle.end();
				for (Package_Bundle::iterator iter = begin; iter != end; ++iter)
				{
					// SW�� ��ȿȭ�ϰ�
					iter->second.sw->invalid();
					// SW�� ����ī��Ʈ�� ���ҽ�Ų��.
					iter->second.sw->release();

					// id�� _hw ������ �ݳ�����.
					my_factory->destruct_hw(iter->first, iter->second.id, &iter->second.hw);
				}

				my_buffer->bundle.clear();

				return S_OK;
			}

			INTERFACE_DECL_FUNC(move)(CONST KEY from, CONST DWORD id, CONST KEY to, ptr_SW * pSw)
			{
				// from �η��� ��� Package�� �ҷ��´�.
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

				// �ɷ��� Package�� Ž���Ѵ�.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// Package�� ã�Ҵٸ� 
					IF_TRUE(iter->second.id == id)
					{
						// iterator
						Package_Bundle::iterator package;

						// to �η��� ���Ҹ� �߰��ϰ�
						package = my_buffer->bundle.insert(Package_Bundle::value_type(to, iter->second));

						IF_NOT_NULL(pSw)
						{
							package->second.sw->addRef();
							(*pSw) = package->second.sw;
						}

						// from �η����� ���Ҹ� �����Ѵ�.
						my_buffer->bundle.erase(iter);

						return S_OK;
					}
				}

				return E_INVALIDARG;
			}

			INTERFACE_DECL_FUNC(move_all)(CONST KEY from, CONST KEY to, LONG count, ptr_SW * pSw)
			{
				// from �η��� ��� Package�� �ҷ��´�.
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
					// �ɷ��� Package�� Ž���Ѵ�.
					for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
					{
						// to �η��� ���Ҹ� �߰��ϰ�
						my_buffer->bundle.insert(Package_Bundle::value_type(to, iter->second));

						++index;
					}
				}
				else
				{
					IF_NOT_NULL(pSw)
					{
						// ���� �ʱ�ȭ
						ZeroMemory(pSw, sizeof(LONG)* count);
					}

					// iterator
					Package_Bundle::iterator SW;

					// �ɷ��� Package�� Ž���Ѵ�.
					for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
					{
						// to �η��� ���Ҹ� �߰��ϰ�
						SW = my_buffer->bundle.insert(Package_Bundle::value_type(to, iter->second));

						IF_TRUE(index < count)
						{
							SW->second.sw->addRef();
							pSw[index] = SW->second.sw;
						}

						++index;
					}
				}

				// Package_Bundle���� from �η��� Package�� ��� �����Ѵ�.
				my_buffer->bundle.erase(list.first, list.second);

				return index;
			}

			INTERFACE_DECL_FUNC_T(DWORD, stock)(CONST KEY key)
			{
				// from �η��� ��� Package�� �ҷ��´�.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// stock
				LONG stock = 0;

				// key �η��� Package ������ Ž���Ѵ�.
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

				// SW_Buffer�� ���Ҹ� ��� �����Ѵ�.
				for (LONG index = buffer.size(); index != -1; --index)
				{
					buffer[index]->release();
				}

				// Package_Bundle�� ũ�⸸ŭ SW_Buffer�� ũ�⸦ �ٽ� �����Ѵ�.
				buffer.resize(stock_all());

				// index
				DWORD index = 0;

				// Package_Bundle�� ������ Package���� SW���� �����ϰ� SW::addRef()�� ȣ���Ѵ�.
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
		$ _finder�� �Ļ� Ŭ����, Embedded_Architecture_Pattern�� �����ϴ� �⺻ _finder Ŭ����
		1. ������ �Ķ������ ���� null�� ������ ���ܸ� ȣ���Ѵ�.
		2. implicit�� ���õ� �Լ��� ����� �޾� ���� ������ �Ѵ�.
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

				// ��ȯ �� �ʱ�ȭ
				(*pSw) = NULL;

				// key �η��� ��� Package�� �ҷ��´�.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// �ɷ��� Package�� ���ؼ� id�� �˻��Ѵ�.
				for (Package_Bundle::iterator iter = list.first; iter != list.second; ++iter)
				{
					// Package�� ã�Ҵٸ� 
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
					// ���� �ʱ�ȭ
					ZeroMemory(pSw, sizeof(LONG)* count);
				}

				// key �η��� ��� Package�� �ҷ��´�.
				std::pair<Package_Bundle::iterator, Package_Bundle::iterator> list
					= my_buffer->bundle.equal_range(key);

				IF_TRUE(list.first == my_buffer->bundle.end())
				{
					return E_INVALIDARG;
				}

				// index
				LONG index = 0;

				// �ɷ��� Package�� ���ؼ� id�� �˻��Ѵ�.
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
		$ ��ȸ�ڸ� ���� �����ϴ� ����� ��ΰ�, ��ȸ ����� �޶� ��ȸ�ڸ� �Ļ� Ŭ������ �����ϰ� �ߴ�.
		*/
	};
};