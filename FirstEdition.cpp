//只能进行正整数运算（被减数不小于减数）
//除法精确到个位
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
using namespace std;

string InputData;

class Expression
{
public:
	int JudValue=1;
	int expressionChange(string exp, vector<int> &expa);
	int NiTransPost(vector<int> expa, vector<int> &expb);
};

int Expression::expressionChange(string exp, vector<int> &expa)//string转换
{
	int n = 0;
	for (int i = 0; i < exp.length(); i++) {
		int expvalue = exp[i];
		if (expvalue <= 57 && expvalue >= 48) {
			if (i != exp.length() - 1)n++;
			else {
				n++;
				int num = 0;
				for (int j = i; j >= i - n + 1; j--)
				{
					expvalue = (int)exp[j];
					num += (expvalue - 48)*pow(10, i - j);
				}
				expa.push_back(num);
			}
		}
		else {
			if (n == 0) {
				expa.push_back(expvalue + 1000000000);
			}
			else {
				int num = 0;
				int temp = expvalue;
				for (int j = i - 1; j >= i - n; j--)
				{
					expvalue = (int)exp[j];
					num += (expvalue - 48)*pow(10, i - 1 - j);
				}
				expa.push_back(num);
				expa.push_back(temp + 1000000000);
				n = 0;
			}
		}

	}
	return 0;
}

int Expression::NiTransPost(vector<int> expa, vector<int> &expb)
{
	int OutStack = 0, InStack = 0;
	vector<int> temp;
	for (int i = 0; i < expa.size(); i++) {
		if (expa[i] <= 1000000000)expb.push_back(expa[i]);
		else if (expa[i] == 1000000000 + 40)temp.push_back(expa[i]);
		else if (expa[i] == 1000000000 + 41) {
			int temp1;
			while (true) {
				temp1 = temp.back();
				temp.pop_back();
				if (temp1 == 1000000000 + 40)break;
				expb.push_back(temp1);
			}
		}
		else if (expa[i] == 1000000000 + 43 || expa[i] == 1000000000 + 45) {
			if (temp.size() == 0) temp.push_back(expa[i]);
			else {
				int temp1;
				while (true)
				{
					temp1 = temp.back();
					if (temp1 == 1000000000 + 40)
					{
						temp.push_back(expa[i]);
						break;
					}
					else if (temp1 == 1000000000 + 43 || temp1 == 1000000000 + 45) {
						expb.push_back(temp.back());
						temp.pop_back();
						temp.push_back(expa[i]);
						break;
					}
					else {
						expb.push_back(temp.back());
						temp.pop_back();
						continue;
					}
				}
			}
		}
		else if (expa[i] == 1000000000 + 42 || expa[i] == 1000000000 + 47) {
			if (temp.size() == 0)temp.push_back(expa[i]);
			else {
				int temp1 = temp.back();
				if (temp1 == 1000000000 + 40 || temp1 == 1000000000 + 43 || temp1 == 1000000000 + 45) {
					temp.push_back(expa[i]);
				}
				else {
					expb.push_back(temp.back());
					temp.pop_back();
					temp.push_back(expa[i]);
				}
			}
		}
	}

	if (temp.size() != 0) {
		int tempsize = temp.size();
		for (int i = 0; i < tempsize; i++) {
			expb.push_back(temp.back());
			temp.pop_back();
		}
	}

	return 0;
}

class ExpressionCalculate:public Expression
{
public:
	vector<int> expa;//正数表示数，负数表示符号
	ExpressionCalculate(){}
	~ExpressionCalculate() {}
	int calculate();
};

int ExpressionCalculate::calculate()
{
	vector<int> expa, expb, expc;
	int operand1 = 0, operand2 = 0;
	if (Expression::JudValue) {
		string exp = InputData;
		Expression::expressionChange(exp, expa);
		Expression::NiTransPost(expa, expb);
		for (int i = 0; i < expb.size(); i++)
		{
			if (expb[i] <= 1000000000)expc.push_back(expb[i]);
			else {
				operand2 = expc.back(); expc.pop_back();
				operand1 = expc.back(); expc.pop_back();
				if (expb[i] == 1000000043)expc.push_back(operand1 + operand2);
				if (expb[i] == 1000000045)expc.push_back(operand1 - operand2);
				if (expb[i] == 1000000042)expc.push_back(operand1 * operand2);
				if (expb[i] == 1000000047) {
					double op1 = (double)operand1; double op2 = (double)operand2;
					double t = op1 / op2; expc.push_back(floor(t));
				}
			}
		}
		cout << "the answer is: " << endl;
		cout << expc.back() << endl;
		return 0;
	}

	return 0;
}

class ExpressionJudge:public Expression
{
public:
	string Warning;

	void Waring_assign(string WarningWord) 
	{
		Warning = WarningWord;
	}
	ExpressionJudge() {}
	~ExpressionJudge() {}
	int Judge();
};
int ExpressionJudge::Judge()
{
	Expression::JudValue = 1;
	int numofleft = 0, numofright = 0;
	int TheLastOperatorPost = 0;
	string exp = InputData;
	for (int i = 0; i < exp.length(); i++) {
		int expvalue = (int)exp[i];
		if ((expvalue < 48 || expvalue>57) && expvalue != 43 && expvalue != 42 && expvalue != 45 && expvalue != 47 && expvalue != 40 && expvalue != 41)//出现非法字符
		{
			cout << Warning << endl;
			Expression::JudValue = 0;
			return 0;
		}

		if (expvalue == 42 || expvalue == 43 || expvalue == 45 || expvalue == 47 || expvalue == 40 || expvalue == 41) {//数字太大
			if ((i - TheLastOperatorPost) > 9) {
				cout << Warning << endl;
				Expression::JudValue = 0;
				return 0;
			}
			TheLastOperatorPost = i + 1;
		}

		if (expvalue == 40) {
			if (i == exp.length() - 1) {//表达式末尾为左括号
				cout << Warning << endl;
				Expression::JudValue = 0;
				return 0;
			}
			int NextExpValue = (int)exp[i + 1];
			if (NextExpValue == 42 || NextExpValue == 43 || NextExpValue == 45 || NextExpValue == 47) {//左括号右边为运算符
				cout << Warning << endl;
				Expression::JudValue = 0;
				return 0;
			}
			if (i != 0) {
				int BeforeExpValue = (int)exp[i-1];
				if (BeforeExpValue != 42 && BeforeExpValue != 43 && BeforeExpValue != 45 && BeforeExpValue != 47) {//左括号左边不是运算符
					cout << Warning << endl;
					Expression::JudValue = 0;
					return 0;
				}
			}
			numofleft++;
		}
		
		if (expvalue == 41) {
			if (i == 0) {//右括号在开头
				cout << Warning << endl;
				Expression::JudValue = 0;
				return 0;
			}
			int BeforeExpValue = (int)exp[i - 1];
			if (BeforeExpValue == 42 || BeforeExpValue == 43 || BeforeExpValue == 45 || BeforeExpValue == 47) {//右括号左边为运算符
				cout << Warning << endl;
				Expression::JudValue = 0;
				return 0;
			}
			int NextExpValue = exp[i+1];
			if (i != exp.length() - 1) {
				if (NextExpValue != 42 && NextExpValue != 43 && NextExpValue != 45 && NextExpValue != 47) {//右括号右边不是运算符
					cout << Warning << endl;
					Expression::JudValue = 0;
					return 0;
				}
			}
			numofright++;
		}

		
	}
	if (numofleft != numofright) {//左括号数与右括号数不相等
		cout << Warning << endl;
		Expression::JudValue = 0;
		return 0;
	}

	int expvalue;
	expvalue = (int)exp[0];
	if (expvalue == 42 || expvalue == 43 || expvalue == 45 || expvalue == 47) {//运算符开头
		cout << Warning << endl;
		Expression::JudValue = 0;
		return 0;
	}

	expvalue = (int)exp[exp.length() - 1];
	if (expvalue == 42 || expvalue  == 43 || expvalue == 45 || expvalue == 47) {//运算符结尾
		cout << Warning << endl;
		Expression::JudValue = 0;
		return 0;
	}
	return 0;

}

class Interface:public ExpressionJudge,ExpressionCalculate
{
public:
	string Hint;
	string WarningWord;
	Interface();
	~Interface() {}
};

Interface::Interface()
{
	Hint = "Input the expression you want to calculate:";
	WarningWord = "Wrong expression!";
	ExpressionJudge::Waring_assign(WarningWord);
	cout << Hint << endl;
	cin >> InputData;
	while (true)
	{
		if (InputData.compare("E") == 0 || InputData.compare("e") == 0)break;
		ExpressionJudge::Judge();
		ExpressionCalculate::calculate();
		cout << Hint << endl;
		cin >> InputData;
	}
}

int main()
{
	Interface a;
	return 0;
}
