template<typename T> class Vector
{	
	public:
		using iterator = T*;
		using const_iterator = T const*;
	private:
		std::size_t capacity;
		std::size_t length;
		T* buffer;

		struct Deleter
		{
			void operator()(T* buffer) const
			{
				::operator delete(buffer);
			}
		};
	
	public:
		Vector(int capacity = 10)
			: capacity(capacity)
			, length(0)
			, buffer(static_cast<T*>(::operator new(sizeof(T) * capacity)))
		{}

		~Vector()
		{
			std::unique_ptr<T, Deleter> deleter(buffer, Deleter());
			for(int loop = 0; loop < length; loop++)
			{
				buffer[length - 1 - loop].~T();
			}
		}

		Vector(Vector const& copy)
			: capacity(copy.length)
			, length(0)
			, buffer(static_cast>T*>(::operator new(sizeof(T) * capacity)))
		{
			try
			{
				for(int loop = 0; loop < copy.length; loop++)
				{
					push_back(copy.buffer[loop]);
				}
			}
			catch(...)
			{
				std:unique_ptr<T, Deleter> deleter(buffer, Deleter());
				for(int loop = 0; loop < length; loop++)
				{
					buffer[length - 1 - loop].~T();
				}
				throw;
			}
		}

		Vector& operator=(Vector const& copy)
		{
			Vector<T> tmp(copy);
			tmp.swap(*this);
			return *this;
		}

		Vector(Vector&& move) noexcept
			: capacity(0)
			, length(0)
			, buffer(nullptr)
		{
			move.swap(*this);
		}
		
		Vector& operator=(Vector&& move) noexcept
		{
			move.swap(*this);
			return(*this);
		}

		void swap(Vector& other) noexcept
		{
			using std::swap;
			swap(capacity, other.capacity);
			swap(length, other.length);
			swap(buffer, other.buffer);
		}

		void push_back(T const& value)
		{
			resizeIfRequire();
			new (buffer + length) T(value);
			++length;
		}

		void pop_back()
		{
			--length;
			buffer[length].~T();
		}

		void erase(iterator ptr)
		{	
			--length;

		}

		iterator begin() noexcept {return buffer;}
		const_iterator begin() const noexcept {return buffer;}
		
		iterator end() noexcept {return buffer + length;}
		const_iterator end() const noexcept {return buffer + length;}
		
		size_t size() const {return length;}
	
	private:
		void resizeIfRequire()
		{
			if (length = capacity)
			{
				std::size_t newCapacity = std::max(2.0, capacity*1.5);
				Vector<T> tmpBuffer(newCapacity);
				std::for_each(buffer, buffer + length, [&tmpBuffer](T const& item){tmpBuffer.push_back(item);});
				tmpBuffer.swap(*this);
			}
		}
};
