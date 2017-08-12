#include <iostream>
#include <memory>

using namespace std;

class Car {
public:
	Car() { cout << "Car constructor!" << endl; }
	~Car() { cout << "Car destructor!!" << endl; }
	void Go() { cout << "Car go!" << endl; }
};

template<typename T> class Ptr {
	T* obj;
public:
	Ptr(T* p = 0) : obj(p) { cout << "Ptr constructor!" << endl; }
	~Ptr() { 
		cout << "Ptr destructor!!" << endl;
		delete obj; 
	}
	T* operator->() { return obj; }
	T& operator*() { return *obj; }
};

shared_ptr<Car> createCar()
{
	return shared_ptr<Car>(new Car());
}

int main()
{
	{
		Ptr<Car> p = new Car;
		(*p).Go();
		p->Go();

		//Ptr<Car> p2 = p;
	}

	cout << endl;

	{
		shared_ptr<Car> c1(new Car);
		shared_ptr<Car> c2 = c1;
		shared_ptr<Car> c3 = createCar();
		c1->Go();
		c2->Go();
		c3->Go();
	}

	return 0;
}
