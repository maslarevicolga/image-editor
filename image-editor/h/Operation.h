#ifndef _OPERATION_H
#define _OPERATION_H

#include <vector>
#include <string>
#include <tuple>
#include <set>
#include <iostream>

class Operation {

public:
	virtual void operator()(std::vector<int>&, const std::set<std::pair<int, int>>&, int);
	virtual Operation* copy(int) { return this; }
	virtual void setParam(int) {}
	virtual std::string toString() const = 0;
	virtual std::string getName()const { return toString(); }
	virtual ~Operation() {};
private:
	virtual void calculate(int&,  int&, int&) {}
};


class Add : public Operation {
	int param;
	Add(int p) : param(p) {}
public:
	Add() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Add(p); }
	std::string getName()const override { return "add(int)"; }
	std::string toString() const override { return "add(" + std::to_string(param) + ")"; }
private:
	void calculate(int&,  int&, int&) override;
};

class Sub : public Operation {
	int param;
	Sub(int p) : param(p) {}
public:
	Sub() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Sub(p); }
	std::string getName()const override { return "sub(int)"; }
	std::string toString() const override { return "sub(" + std::to_string(param) + ')';  }
private:
	void calculate(int&,  int&, int&) override;
};

class Sub_Inv : public Operation {
	int param;
	Sub_Inv(int p) : param(p) {}
public:
	Sub_Inv() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Sub_Inv(p); }
	std::string getName()const override { return "sub_inverse(int)"; }
	std::string toString() const override { return "sub_inverse(" + std::to_string(param) + ')'; }
private:
	void calculate(int&,  int&, int&) override;
};

class Mul : public Operation {
	int param;
	Mul(int p) : param(p) {}
public:
	Mul() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Mul(p); }
	std::string getName()const override { return "mul(int)"; }
	std::string toString() const override { return "mul(" + std::to_string(param) + ')';}
private:
	void calculate(int&,  int&, int&) override;
};

class Div : public Operation {
	int param;
	Div(int p) : param(p) {}
public:
	Div() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Div(p); }
	std::string getName()const override { return "div(int)"; }
	std::string toString() const override { return "div(" + std::to_string(param) + ')';}
private:
	void calculate(int&,  int&, int&) override;
};

class Div_Inv : public Operation {
	int param;
	Div_Inv(int p) : param(p) {}
public:
	Div_Inv() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Div_Inv(p); }
	std::string getName()const override { return "div_inverse(int)"; }
	std::string toString() const override { return "div_inverse(" + std::to_string(param) + ')'; }
private:
	void calculate(int&,  int&, int&) override;
};

class Pow : public Operation {
	int param;
	Pow(int p) : param(p) {}
public:
	Pow() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Pow(p); }
	std::string getName()const override { return "pow(int)"; }
	std::string toString() const override { return "pow(" + std::to_string(param) + ')'; }
private:
	void calculate(int&,  int&, int&) override;
};

class Log : public Operation {
	
public:
	std::string toString() const override { return "log()"; }
private:
	void calculate(int&,  int&, int&) override;
};

class Abs : public Operation {
	
public:
	std::string toString() const override { return "abs()"; }
private:
	void calculate(int&,  int&, int&) override;
};

class Max : public Operation {
	int param;
	Max(int p) : param(p) {}
public:
	Max() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Max(p); }
	std::string getName()const override { return "max(int)"; }
	std::string toString() const override { return "max(" + std::to_string(param) + ')'; }
private:
	void calculate(int&,  int&, int&) override;
};

class Min : public Operation {
	int param;
	Min(int p) : param(p){}
public:
	Min() = default;
	void setParam(int p) { param = p; }
	Operation* copy(int p) override { return new Min(p); }
	std::string getName()const override { return "min(int)"; }
	std::string toString() const override { return "min(" + std::to_string(param) + ')'; }
private:
	void calculate(int&,  int&, int&) override;
};

class Inversion : public Operation {

public:
	std::string toString() const override { return "inversion()"; }
private:
	void calculate(int&,  int&, int&) override;
};

class Grayscale : public Operation {
	
public:
	std::string toString()const override { return "grayscale()"; }
private:
	void calculate(int&,  int&, int&) override;
};

class Black_White : public Operation {
	
public:
	std::string toString()const override { return "black_white()"; }
private:
	void calculate(int&,  int&, int&) override;
};

class Median : public Operation {
	
public:
	std::string toString()const override { return "median()"; }
	void operator()(std::vector<int>&, const std::set<std::pair<int, int>>&, int) override;
private:
	void calculate(std::vector<int>&, std::vector<int>&, int, int, int, int);
};

class Composite : public Operation {
	std::string name;
	std::vector<Operation*> operations;
public:
	Composite(std::vector<Operation*> op, const std::string& n) : operations(op), name(n){}
	void operator()(std::vector<int>&, const std::set<std::pair<int, int>>&, int) override;
	
	std::string toString() const override{ return name + "()"; };

	std::vector<Operation*>::iterator begin() { return operations.begin(); }
	std::vector<Operation*>::iterator end() { return operations.end(); }

	std::vector<std::string> saveFunction() const;
	void remove(const std::string&);
	friend std::ostream& operator<<(std::ostream& os, const Composite& c){
		for (auto& it : c.operations) {
			os << it->toString();
			os << " ";
		}
		return os;
	}


	~Composite();

};

#endif