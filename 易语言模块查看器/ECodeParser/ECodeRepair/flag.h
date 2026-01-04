#pragma once

#include <vcruntime_string.h>

class flag_t
{
private:
	class flag_bool
	{
	public:
		flag_bool(flag_t *self, size_t i, size_t x)
		{
			this->_self = self;
			this->_i = i;
			this->_x = x;
			this->_value = (self->_ptr[i] >> x) & 1;
		}

		flag_bool &operator =(const bool value)
		{
			this->_value = value;
			this->_self->_ptr[this->_i] |= (value & 1) << this->_x;
			return *this;
		}

		operator bool()
		{
			return this->_value;
		}

	private:
		flag_t *_self;
		size_t _i;
		size_t _x;
		bool _value;
	};

public:
	flag_t(size_t size)
	{
		this->_size = size;
		this->_max = size / 8;
		if (size % 8 != 0)
			this->_max++;
		this->_ptr = new unsigned char[this->_max];
		memset(this->_ptr, 0, this->_max);
	}

	~flag_t()
	{
		delete[] this->_ptr;
	}

	void set_all(bool value)
	{
		memset(this->_ptr, value ? 0xff : 0, this->_max);
	}

	size_t get_value_count(bool value)
	{
		size_t count = 0;
		for (size_t i = 0; i < this->_size; i++)
			if (this->operator[](i) == value)
				count++;
		return count;
	}

	flag_t &operator =(const flag_t &self)
	{
		this->_size = self._size;
		this->_max = self._max;
		delete[] this->_ptr;
		this->_ptr = new unsigned char[this->_max];
		memcpy(this->_ptr, self._ptr, self._max);
		return *this;
	}

	flag_bool operator[](const size_t pos)
	{
		return flag_bool(this, pos / 8, pos % 8);
	}

	flag_t &operator++(int)
	{
		for (size_t i = 0; i < this->_max; i++)
		{
			if (this->_ptr[i] == 0xff)
				this->_ptr[i] = 0;
			else
			{
				this->_ptr[i]++;
				break;
			}
		}
		return *this;
	}

	flag_t &operator--()
	{
		for (size_t i = 0; i < this->_max; i++)
		{
			if (this->_ptr[i] == 0)
				this->_ptr[i] = 0xff;
			else
			{
				this->_ptr[i]--;
				break;
			}
		}
		return *this;
	}

private:
	size_t _size;
	size_t _max;
	unsigned char *_ptr;
};