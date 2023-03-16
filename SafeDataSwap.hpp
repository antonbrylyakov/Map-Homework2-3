#include <mutex>

namespace SafeDataSwap
{
	template <typename T>
	class Data
	{

	// Обмен данными с использованием lock
	template <typename N>
	friend void exchangeDataLk(Data<N>& d1, Data<N>& d2);

	// Обмен данными с использованием unique_lock
	template <typename N>
	friend void exchangeDataUl(Data<N>& d1, Data<N>& d2);

	// Обмен данными с использованием scoped_lock
	template <typename N>
	friend void exchangeDataSl(Data<N>& d1, Data<N>& d2);

	// Обмен данными, который может привести к взаимоблокировке
	template <typename N>
	friend void exchangeDataDeadlock(Data<N>& d1, Data<N>& d2);

	public:
		Data(T value) :m_value(value), m_lock()
		{
		}

		// Не совсем понятно, что значит "скалярные данные".
		// Считаем, что допустимо возвращать из по значению
		T getValue() const
		{
			return m_value;
		}
	private:
		std::mutex m_lock;
		T m_value;
	};


	template<typename N>
	void exchangeDataLk(Data<N>& d1, Data<N>& d2)
	{
		std::lock(d1.m_lock, d2.m_lock);
		std::lock_guard<std::mutex> lk1(d1.m_lock, std::adopt_lock);
		std::lock_guard<std::mutex> lk2(d2.m_lock, std::adopt_lock);

		std::swap(d1.m_value, d2.m_value);
	}

	template<typename N>
	void exchangeDataUl(Data<N>& d1, Data<N>& d2)
	{
		std::unique_lock<std::mutex> lk1(d1.m_lock, std::defer_lock);
		std::unique_lock<std::mutex> lk2(d2.m_lock, std::defer_lock);
		std::lock(lk1, lk2);

		std::swap(d1.m_value, d2.m_value);
	}

	template<typename N>
	void exchangeDataSl(Data<N>& d1, Data<N>& d2)
	{
		std::scoped_lock lock(d1.m_lock, d2.m_lock);

		std::swap(d1.m_value, d2.m_value);
	}

	// Этот вариант на тестах приводит к взаимоблокировке
	template<typename N>
	void exchangeDataDeadlock(Data<N>& d1, Data<N>& d2)
	{
		std::lock_guard<std::mutex> lk1(d1.m_lock);
		std::lock_guard<std::mutex> lk2(d2.m_lock);

		std::swap(d1.m_value, d2.m_value);
	}

}

