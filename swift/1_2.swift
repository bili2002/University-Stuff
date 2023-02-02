import Swift

extension StringProtocol {
    subscript(offset: Int) -> Character {
        return self[index(startIndex, offsetBy: offset)]
    }
}

extension Double {
	static func ^ (number: Double, power: Double) -> Double {
		var ans = number;
		for _ in 2...Int(power) {
			ans *= number;
		}
		
		return ans;
	}
}


func isNum(character: Character) -> Bool {
	if character >= "0" && character <= "9" {
		return true
	}
	return false
}

func findNum(expression: String, begin: Int) -> (value: Double, end: Int) {
	var value: Double = 0.0, curr = begin
	while curr < expression.count && isNum(character: expression[curr]) {
		if let temp = Int(String(expression[curr])) {
			value = value*10 + Double(temp)
		}
		curr = curr+1
	} 
	return (value, curr-1)
}

func calculate(firstNum: Double, secondNum:Double, sign: Character) -> Double {
	var ans: Double = 0.0
	
	switch sign {
	case "+":
		ans = firstNum + secondNum
	case "-": 
		ans = firstNum - secondNum
	case "*": 
		ans = firstNum * secondNum
	case "/": 
		ans = firstNum / secondNum
	default: 
		ans = firstNum ^ secondNum
	}
	
	return ans
}

func isSign(char: Character) -> Bool {
	if char == "+" || char == "-" || char == "*" || char == "/" || char == "^" {
		return true
	}
	return false
}

func evaluate(expression: String, begin: Int) -> (value: Double, end: Int) {

	var curr = begin 
	
	var sign: Character = "+"
	var firstNum = false
	var currNum: Double = 0.0, prevNum: Double = 0.0
	while expression[curr] != ")" {
		if expression[curr] == "(" {
			(currNum, curr) = evaluate(expression: expression, begin: curr+1)
			if firstNum == false {
				prevNum = currNum
				firstNum = true
			}
		} else if isNum(character: expression[curr]) {
			(currNum, curr) = findNum(expression: expression, begin: curr)
			if firstNum == false {
				prevNum = currNum
				firstNum = true
			}
		} else if isSign(char: expression[curr]) {
			sign = expression[curr]
		}
		curr += 1
		
	} 
	
	
	let ans = calculate(firstNum: prevNum, secondNum: currNum, sign: sign)
	return (ans, curr)
}

func evaluate(expression: String) -> Double {
	if expression.isEmpty {
		return 0
	}
	
	if expression[0] != "(" {
		return findNum(expression: expression, begin: 0).value
	}
	
	return evaluate(expression: expression, begin: 1).value
}
/*
var expression: String =  "  "
print(evaluate(expression: expression))*/
