#pragma once

namespace MetaProg
{

	//Definition for calling a metafunction
	//metafunctions are types containing a templated member called 'apply'
	//which takes the function parameters as template arguments
	//the return value should be in a typedef called 'type'
	template <typename F, typename... Args>
	using eval = typename F::template apply<Args...>::type;


	//This type represents an empty list
	struct Nil {};

	//This is the main lazy list
	//Head should be the head of the current list
	//GetTail is a metafunction that will return the rest of the list
	//as a lazy list itself, or alternatively Nil, when called with 'Head' as a parameter
	template <typename Head, typename GetTail>
	struct LazyList
	{
		using head = Head;
		using tail = eval<GetTail, Head>;
	};

	//common syntax for interacting with elements of a list
	//'car' denotes the head of the list, while 
	//'cdr' denotes the tail (which will actually be evaluated to retrieve it)
	//asking for the head or tail of an empty list will be an error
	template <typename L>
	using car = typename L::head;
	template <typename L>
	using cdr = typename L::tail;

	//basic get-tail metafunction returning a constant value
	template <typename Val>
	struct Constant
	{
		//while we don't care what the current head is, we still have to take
		//the parameter
		template <typename>
		struct apply
		{
			using type = Val;
		};
	};

	//wrapper around prepending to a lazy list
	template <typename H, typename L>
	using cons = LazyList<H, Constant<L>>;

	//helper that will help construct a lazy list from a variadic template list
	template <typename... Vals>
	struct ToLazyList;

	template <typename H, typename... Vals>
	struct ToLazyList<H, Vals...>
	{
		using type = cons<H,typename ToLazyList::type>;
	};

	template <>
	struct ToLazyList<>
	{
		using type = Nil;
	};

	template <typename... Values>
	using to_lazy_list = typename ToLazyList<Values...>::type;

}
