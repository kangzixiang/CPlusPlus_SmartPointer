/**
*
*
*
*
**/
#pragma once

template<class T>
class ref_ptr
{

public:
	typedef T element_type;

	ref_ptr() : _ptr(0, _refCount(0) {}
	ref_ptr(T* ptr) : _ptr(ptr), _refCount(0) 
	{ 
		if (_ptr) 
			++_refCount; 
	}
	ref_ptr(const ref_ptr& rp) : _ptr(rp._ptr), _refCount(rp._refCount)
	{ 
		if (_ptr)
		{
			++_refCount;
			rp._refCount = _refCount;
		}
	}
	template<class Other> ref_ptr(const ref_ptr<Other>& rp) : _ptr(rp._ptr), _refCount(rp._refCount)
	{ 
		if (_ptr)
		{
			++_refCount;
			rp._refCount = _refCount;
		}
	}
	~ref_ptr() 
	{ 
		if (_ptr)
		{
			--_refCount;  
			_ptr = 0;
			if (_refCount == 0)
			{
				delete _ptr;
				_ptr = NULL;
			}
		}
	}

	ref_ptr& operator = (const ref_ptr& rp)
	{
		assign(rp);
		return *this
	}

	template<class Other> ref_ptr& operator = (const ref_ptr<Other>& rp)
	{
		assign(rp);
		return *this;
	}

	inline ref_ptr& operator = (T* ptr)
	{
		if (_ptr == ptr)
		{
			return *this;
		}
		T* tmp_ptr = _ptr;
		unsigned int tmp_refCount = _refCount;
		_ptr = ptr;
		if (_ptr)
		{
			_refCount = 0;
			++_refCount;
		}
		if (tmp_ptr)
		{
			--tmp_refCount;
			if (tmp_refCount == 0)
			{
				delete tmp_ptr;
				tmp_ptr = NULL;
			}
		}
		return *this;
	}

	bool operator == (const ref_ptr& rp) const 
	{ 
		return (_ptr == rp._ptr); 
	}

	bool operator == (const T* ptr) const 
	{ 
		return (_ptr == ptr); 
	}

	friend bool operator == (const T* ptr, const ref_ptr& rp) 
	{ 
		return (ptr == rp._ptr); 
	}

	bool operator != (const ref_ptr& rp) const 
	{ 
		return (_ptr != rp._ptr); 
	}

	bool operator != (const T * ptr) const 
	{ 
		return (_ptr != ptr); 
	}

	friend bool operator != (const T * ptr, const ref_ptr & rp) 
	{ 
		return (ptr != rp._ptr); 
	}

	bool operator < (const ref_ptr & rp) const 
	{ 
		return (_ptr < rp._ptr); 
	}

	operator T* () const { return _ptr; }

	T& operator*() const
	{
		if (!_ptr) {
			// pointer is invalid, so throw an exception
			throw std::runtime_error(std::string("could not dereference invalid osg pointer ") + std::string(typeid(T).name()));
		}
		return *_ptr;
	}

	T* operator->() const
	{
		if (!_ptr) {
			// pointer is invalid, so throw an exception.
			throw std::runtime_error(std::string("could not call invalid osg pointer ") + std::string(typeid(T).name()));
		}
		return _ptr;
	}

	bool valid() const { return _ptr != 0; }

	T* get() const { return _ptr; }
	T* release() 
	{ 
		T* tmp = _ptr;
		unsigned int tmp_refCount = _refCount;

		if (_ptr && _refCount)
		{
			--_refCount;
			_ptr = 0; 
		}
		
		return tmp; 
	}

	void swap(ref_ptr& rp) 
	{ 
		T* tmp = _ptr;
		unsigned int tmp_refCount = _refCount;

		_ptr = rp._ptr;
		_refCount = rp._refCount;

		rp._ptr = tmp;
		rp._refCount = tmp_refCount;
	}

private:

	template<class Other> void assign(const ref_ptr<Other>& rp)
	{
		if (_ptr == rp._ptr) return;
		
		T* tmp_ptr = _ptr;
		unsigned int tmp_refCount = _refCount;
		
		_ptr = rp._ptr;
		_refCount = rp._refCount;
		
		if (_ptr)
		{
			++_refCount;
			rp._refCount = _refCount;
		}

		if (tmp_ptr)
		{
			--tmp_refCount;
			if (tmp_refCount == 0)
			{
				delete tmp_ptr;
				tmp_ptr = NULL;
			}
		}
		
	}

	template<class Other> friend class ref_ptr;

private:
	T* _ptr;
	unsigned int _refCount;
};