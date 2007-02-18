#pragma once
///COM interface smart pointer.

///The com_ptr encapsulates the COM interface pointer as a "smart pointer". It is conceptually similar to
///boost::counted_ptr, but based on COM reference counting. While it looks like Microsoft ATL's _com_ptr_t,
///it is far simpler with fewer loopholes and gotchas.

///AddRef is always only done at construction and Release at destruction, thus typical C++ usage takes care
///of reference-counting issues. Swaps, resets and assignment operators all create internal temporaries to
///preserve these invariants, ensuring the object does not leak references.

///Comparisons use COM equality rules: the interface pointers are converted to IUnknown pointers for
///comparison.

///@param	T	The COM interface type.

namespace core
{
	template <typename T>
	class com_ptr
	{
	public:
		/// The COM interface type.
		typedef T	element_type;

		/// Constructs from a raw COM interface @a ptr.
		explicit com_ptr(element_type *ptr = NULL)
			: ptr_(ptr)
		{
			if (ptr_)
				ptr_->AddRef();
		}

		/// Resets COM pointer
		void reset(element_type *ptr = 0)
		{
			if (ptr_)
				ptr->Release();
			ptr_ = ptr;
			if (ptr_)
				ptr_->AddRef();
		}	

		/// Destructs the pointer.
		~com_ptr()
		{
			if (ptr_)
				ptr_->Release();
		}


		/// Returns a reference to the COM type.
		element_type& operator *() const
		{
			return *ptr_;
		}

		element_type** operator &()
		{
			return &ptr_;
		}

		/// Returns a pointer to the COM type.
		element_type * operator->() const
		{
			return ptr_;
		}

		/// Returns the raw interface pointer.
		element_type * get() const
		{
			return ptr_;
		}

		operator bool() const {return NULL != ptr_; }

	private:
		T	*ptr_;
	};
} // namespace core;