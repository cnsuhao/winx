/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/Map.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Map.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MAP_H__
#define __STDEXT_MAP_H__

#ifndef _MAP_
#include <map>
#endif

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class Map

template <
	class KeyT, class DataT,
	class PredT = std::less<KeyT>,
	class AllocT = ScopeAlloc
	>
class Map : public std::map< KeyT, DataT, PredT, StlAlloc<DataT, AllocT> >
{
private:
	typedef StlAlloc<DataT, AllocT> _Alloc;
	typedef std::map<KeyT, DataT, PredT, _Alloc> _Base;

public:
	explicit Map(AllocT& alloc, const PredT& pred = PredT())
		: _Base(pred, alloc)
	{
	}

	template <class Iterator>
	Map(AllocT& alloc, Iterator first, Iterator last, const PredT& pred = PredT())
		: _Base(first, last, pred, alloc)
	{
	}
};

// -------------------------------------------------------------------------
// class MultiMap

template <
	class KeyT, class DataT,
	class PredT = std::less<KeyT>,
	class AllocT = ScopeAlloc
	>
class MultiMap : public std::multimap< KeyT, DataT, PredT, StlAlloc<DataT, AllocT> >
{
private:
	typedef StlAlloc<DataT, AllocT> _Alloc;
	typedef std::multimap<KeyT, DataT, PredT, _Alloc> _Base;

public:
	explicit MultiMap(AllocT& alloc, const PredT& pred = PredT())
		: _Base(pred, alloc)
	{
	}

	template <class Iterator>
	MultiMap(AllocT& alloc, Iterator first, Iterator last, const PredT& pred = PredT())
		: _Base(first, last, pred, alloc)
	{
	}
};

// -------------------------------------------------------------------------
// class TestMap

template <class LogT>
class TestMap : public TestCase
{
	WINX_TEST_SUITE(TestMap);
		WINX_TEST(testMap);
		WINX_TEST(testMultiMap);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	class Obj
	{
		WINX_DECLARE_COUNT(Obj);
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
			printf("construct Obj: %d\n", m_val);
		}
		Obj(const Obj& o) {
			m_val = o.m_val;
			printf("copy constructor: %d\n", m_val);
		}
		~Obj() {
			printf("destruct Obj: %d\n", m_val);
		}
	};

	void testMap(LogT& log)
	{
		std::BlockPool recyle;
		std::ScopeAlloc alloc(recyle);
		std::Map<int, Obj> coll(alloc);
		coll.insert(std::Map<int, Obj>::value_type(1, 2));
		coll.insert(std::Map<int, Obj>::value_type(1, 4));
		coll.insert(std::Map<int, Obj>::value_type(2, 4));
		coll.insert(std::Map<int, Obj>::value_type(4, 8));
		AssertExp(coll.size() == 3);
	}

	void testMultiMap(LogT& log)
	{
		std::BlockPool recyle;
		std::ScopeAlloc alloc(recyle);
		std::MultiMap<int, Obj> coll(alloc);
		coll.insert(std::Map<int, Obj>::value_type(1, 2));
		coll.insert(std::Map<int, Obj>::value_type(1, 4));
		coll.insert(std::Map<int, Obj>::value_type(2, 4));
		coll.insert(std::Map<int, Obj>::value_type(4, 8));
		AssertExp(coll.size() == 4);
	}

public:
	enum { N = 20000 };

	void doStlMap(LogT& log)
	{
		typedef std::map<int, int> MapT;
		log.print("===== std::map =====\n");
		std::PerformanceCounter counter;
		{
			MapT coll;
			for (int i = 0; i < N; ++i)
				coll.insert(MapT::value_type(i, i));
		}
		counter.trace(log);
	}

	void doMap1(LogT& log)
	{
		typedef std::Map<int, int, std::less<int>, std::AutoFreeAlloc> MapT;
		log.print("===== std::Map (AutoFreeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::AutoFreeAlloc alloc;
			MapT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.insert(MapT::value_type(i, i));
		}
		counter.trace(log);
	}

	void doMap2(LogT& log)
	{
		typedef std::Map<int, int> MapT;
		log.print("===== std::Map (ScopeAlloc) =====\n");
		std::PerformanceCounter counter;
		{
			std::BlockPool recyle;
			std::ScopeAlloc alloc(recyle);
			MapT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.insert(MapT::value_type(i, i));
		}
		counter.trace(log);
	}

	void doShareAllocMap(LogT& log)
	{
		typedef std::Map<int, int> MapT;
		std::BlockPool recyle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocMap =====\n");
			std::PerformanceCounter counter;
			{
				std::ScopeAlloc alloc(recyle);
				MapT coll(alloc);
				for (int i = 0; i < N; ++i)
					coll.insert(MapT::value_type(i, i));
			}
			counter.trace(log);
		}
	}

	void testCompare(LogT& log)
	{
		for (int i = 0; i < 5; ++i)
		{
			log.newline();
			doStlMap(log);
			doMap2(log);
			doMap1(log);
		}
		doShareAllocMap(log);
	}
};

// -------------------------------------------------------------------------
// $Log: Map.h,v $

__NS_STD_END

#endif /* __STDEXT_MAP_H__ */