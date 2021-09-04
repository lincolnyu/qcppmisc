#pragma once

#include <functional>
#include <ostream>
#include <stdexcept>
#include <vector>


namespace qml
{
	template <typename T>
	class Hereditary
	{
	public:
		struct Term
		{
			Hereditary<T>	node;
			T				coeff;
			bool			continual = false;

			Term(T coeff, Hereditary<T> node, bool continual = false)
				: coeff(coeff), node(std::move(node)), continual(continual)
			{
			}

			bool isConst() const
			{
				return node.isZero();
			}

			bool isFirstOrder() const
			{
				return node.isConst() && node.terms()[0].coeff == 1;
			}

			bool operator==(const Term& other) const
			{
				return continual == other.continual && coeff == other.coeff && node == other.node;
			}
		};
		
	private:
		bool isTermSecondOrder(const Term& t) const
		{
			if (t.node.isConst() && t.node.terms()[0].coeff == 2)
			{
				return true;
			}
			if (_base == 2)
			{
				// t.node has to represent 2
				if (t.node.terms().size() == 1)
				{
					return t.node.terms()[0].isFirstOrder();
				}
			}
			return false;
		}
	public:
		Hereditary(T initial_value);

		bool isZero() const;
		bool isConst() const;

		T base() const;
		void incBase();
		const std::vector<Term>& terms() const;

		// Pre: this is equal or greater than that
		// Post: Return the diff if the diff is less than max
		int diff(Hereditary<T> that, int max=3) const;

		bool operator==(const Hereditary<T>& other) const;
		Hereditary& operator--();
		Hereditary& operator++();


		//friend std::ostream& operator<<(std::ostream& out, const Heriditary<T>& h);
		
	private:
		std::vector<Term> _terms;	// least significant to most significant
		T			      _base;
	};

	template <typename T>
	bool Hereditary<T>::isZero() const
	{
		return _terms.size() == 0;
	}

	template <typename T>
	bool Hereditary<T>::isConst() const
	{
		if (isZero())
		{
			return true;
		}
		return _terms.size() == 1 && _terms.begin()->node.isZero();
	}

	template <typename T>
	T Hereditary<T>::base() const
	{
		return _base;
	}

	template <typename T>
	void Hereditary<T>::incBase()
	{
		++_base;
		for (auto& t : _terms)
		{
			t.node.incBase();
		}
	}

	template <typename T>
	const std::vector<typename Hereditary<T>::Term>& Hereditary<T>::terms() const
	{
		return _terms;
	}

	template <typename T>
	Hereditary<T>::Hereditary(T initial_value)
		: _base{2}
	{
		T p = 0;
		int state = 0;
		for (; initial_value > 1; initial_value >>= 1, ++p)
		{
			if (initial_value & 1)
			{
				if (state == 0)
				{
					_terms.emplace_back(1, Hereditary<T>{ p });
					state = 1;
				}
				else if (state < 3)
				{
					state++;
				}
			}
			else
			{
				if (state > 1)
				{
					_terms.emplace_back(1, Hereditary<T>{ p }, state > 2);
				}
				state = 0;
			}
		}
		if (initial_value == 1) // MSB
		{
			_terms.emplace_back(1, Hereditary<T>{ p }, state > 2);
		}
	}

	template <typename T>
	int Hereditary<T>::diff(Hereditary<T> that, int max) const
	{
		for (auto i = 0; i < max; i++)
		{
			if (*this == that)
			{
				return i;
			}
		}
		return -1;
	}

	template <typename T>
	bool Hereditary<T>::operator==(const Hereditary<T>& other) const
	{
		if (_base != other._base)
		{
			return false;
		}
		return _terms == other._terms;
	}

	template <typename T>
	Hereditary<T>& Hereditary<T>::operator++()
	{

	}

	template <typename T>
	Hereditary<T>& Hereditary<T>::operator--()
	{
		if (isZero())
		{
			throw std::underflow_error("Decreasing zero");
		}
		if (_terms[0].isConst())
		{
			if (_terms.size() > 1 && _terms[1].continual)
			{
				if (_terms[1].isFirstOrder())
				{
					_terms[1].continual = false;
				}
				else // > 1-order
				{
					_terms[1].continual = !isTermSecondOrder(_terms[1]); // higher than 2-order
					auto&& c = _terms[1].coeff;
					_terms.insert(_terms.begin() + 1, Term{ c, Hereditary<T>{1} });
				}
			}
			if (--_terms[0].coeff == 0)
			{
				_terms.erase(_terms.begin());
			}
		}
		else
		{
			if (_terms.size() > 1)
			{
				tlast = _terms[0].node;
				++tlast;
				auto diff = _terms[1].node.diff(tlast);
				if (diff == 0)
				{
					// add 
					_terms[1].continual = false;
				}
				else if (diff == 1)
				{
					// add one down _terms[1]
					_terms[1].continual = false;
				}
				else
				{
					// add one
				}

				// TODO fix me...
			}

			// k M^n
			// => k M^n - 1
			//	= (k - 1) M ^ n + M ^ n - 1
			//	= (k - 1) M ^ n + (M - 1) M ^ (n - 1) + ... + (M - 1)
			// continual: k M^? ... k M^(n+1)
			//	M ^ n
			// => M ^ n - 1
			//  = (M - 1) M ^ (n - 1) + ... + (M - 1)
			// continual: M^? ... M^(n+1)

			auto m = _base;
			--m;

			auto&& b = *_terms.begin();
			if (b.coeff > 1)
			{
				auto cb = b;
				--cb.coeff;
				_terms.insert(_terms.begin() + 1, cb);
			}
			--b.node;
			b.coeff = m;
			if (!b.node.isZero())
			{
				auto b_node_gt_1 = (!b.node.isConst() || b.node.terms()[0].coeff > 1);
				b.continual = b_node_gt_1;
				_terms.insert(_terms.begin(), Term{ m, Hereditary<T>{0} });
			}
		}
		return *this;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& out, const Hereditary<T>& h)
	{
		if (h.isZero())
		{
			out << "0";
			return out;
		}
		bool first = true;
		for (const auto& t : h.terms())
		{
			if (t.continual)
			{
				out << "+...+";
			}
			else if (!first)
			{
				out << "+";
			}
			out << t.coeff;
			if (!t.node.isZero())
			{
				out << "*" << h.base() << "^";
				if (!t.node.isConst())
				{
					out << "(" << t.node << ")";
				}
				else
				{
					out << t.node;
				}
			}

			first = false;
		}
		return out;
	}
}
