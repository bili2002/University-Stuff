protocol AnyList {
    func flattenToArray() -> [Any]
}

class List<T> : AnyList {
    var value: T
    var next: List<T>?

    init (at item: Any) {
        self.value = item as! T
    }

    init(_ items: Any...) {
        self.value = items[0] as! T

        var firstEl = false
        var curr = self

        for item in items {
            if !firstEl {
                firstEl = true
                continue
            }

            curr.next = List<T>(at: item)
            if let temp2 = curr.next {
                curr = temp2
            }
        }
    }
}

extension List {
    func create(_ items: [Any]) -> List {
        let newList : List<T> = List<T>(at: items[0])

        var firstEl = false
        var curr = newList

        for item in items {
            if !firstEl {
                firstEl = true
                continue
            }

            curr.next = List<T>(at: item)
            if let temp2 = curr.next {
                curr = temp2
            }
        }
        return newList
    }
}

extension List {
    subscript(index: Int) -> T? {
        var curr = self;
        for _ in 0..<index {
            if let temp = curr.next {
                curr = temp
            } else {
                return nil
            }
        }
        return curr.value
    }
}

extension List {
    var length: Int {
        var curr = self
        var cnt = 1

        while let temp = curr.next {
            cnt += 1
            curr = temp
        }
        return cnt
    }
}

extension List {
    func reverse() {
        var arr = [T]()

        var curr = self
        while let temp = curr.next {
            arr.append(curr.value)
            curr = temp
        }
        arr.append(curr.value)

        curr = self
        while let temp = curr.next {
            curr.value = arr.removeLast()
            curr = temp
        }
        curr.value = arr.removeLast()
    }
}

extension List {
    func flattenToArray() -> [Any] {
        var arr = [Any]()

        var curr = self
        var currVal = curr.value
        while let temp = curr.next {
            if let currVal = currVal as? AnyList {
                arr.append(contentsOf: currVal.flattenToArray())
            } else {
                 arr.append(currVal)
            }
            curr = temp
            currVal = curr.value
            //print(String(arr.last as! Int))
        }
        if let currVal = currVal as? AnyList {
            arr.append(contentsOf: currVal.flattenToArray())
        } else {
                arr.append(currVal)
        }

        return arr
    }
}

extension List {
    func flatten() -> List {
        return create(self.flattenToArray())
    }
}

//var p =  List<Any>(List<Int>(2, 2), 21, List<Any>(3, List<Int>(5, 8))).flatten()


