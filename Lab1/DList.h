#pragma once

template<typename T>
class DList
{

public:
	struct Node
	{
		T m_data;
		Node *prev, *next;

		Node(T _data, Node *p = nullptr, Node *n = nullptr)
		{
			m_data = _data;
			prev = p;
			next = n;
		}
	};
	DList();
	~DList();
	void push_back(const T& _data);
	void pop_back();
	void push_front(const T& _data);
	bool erase(unsigned int _index);
	void clear();
	bool IsEmpty() const;
	unsigned int size() const;



	T& operator[](int index);
	const T& operator[](int index) const;

private:
	Node *m_first, *m_last;
	unsigned int m_size;
};


template<typename T>
DList<T>::DList()
{
	m_first = nullptr;
	m_last = nullptr;
	m_size = 0;
}
template<typename T>
DList<T>::~DList()
{
	for (size_t i = 0; i < m_size; i++)
	{
		pop_back();
	}
}
template<typename T>
void DList<T>::push_back(const T& _data)
{
	Node *newNode = new Node(_data, m_last);

	if (m_first == nullptr)
	{
		m_first = newNode;
	}
	else
	{
		m_last->next = newNode;
	}

	m_last = newNode;

	m_size++;
}
template<typename T>
void DList<T>::pop_back()
{
	if (IsEmpty())
		return;

	Node *delNode = m_last;

	if (m_last == m_first)
	{
		m_first = m_last = nullptr;
	}
	else
	{
		m_last = m_last->prev;
		m_last->next = nullptr;
	}

	delete delNode;

	m_size--;
}
template<typename T>
bool DList<T>::IsEmpty() const
{
	return m_size == 0;
}
template<typename T>
unsigned int DList<T>::size() const
{
	return m_size;
}
template<typename T>

void DList<T>::push_front(const T& info)
{
	Node *newNode = new Node(info, nullptr, m_first);

	if (m_first == nullptr)
	{
		m_last = newNode;
	}
	else
	{
		m_first->prev = newNode;
	}

	m_first = newNode;

	m_size++;
}
template<typename T>
bool DList<T>::erase(unsigned int _index)
{
	Node *travNode;
	if (_index > m_size - 1)
		return false;

	else if (m_size == 1)
	{
		pop_back();
	}
	else if (_index == 0)
	{
		travNode = m_first;
		m_first->next->prev = nullptr;
		m_first = m_first->next;
		delete travNode;
		m_size--;
		return true;
	}
	else if (_index == m_size - 1)
	{
		travNode = m_last;
		m_last->prev->next = nullptr;
		m_last = m_last->prev;
		delete travNode;
		m_size--;
		return true;
	}

	else if (_index > 0 && _index < m_size - 1)
	{
		travNode = m_first;
		for (unsigned int i = 0; i < _index; i++)
		{
			travNode = travNode->next;
		}
		travNode->prev->next = travNode->next;
		delete travNode;
		m_size--;
		return true;
	}

	return false;

}
template<typename T>
void DList<T>::clear()
{
	Node * travNode = m_first;
	for (; m_size;)
	{
		pop_back();
	}
}

template<typename T>
T& DList<T>::operator[](int index)
{
	Node *travNode = m_first;

	for (int i = 0; i < index; i++) travNode = travNode->next;

	return travNode->m_data;
}
template<typename T>
const T& DList<T>::operator[](int index) const
{
	Node *travNode = first;

	for (int i = 0; i < index; i++) travNode = travNode->next;

	return travNode->m_data;
}