protocol Fillable {
    var color: String { set get }
}

protocol VisualComponent: Fillable {
    var boundingBox: Rect { get }
    var parent: VisualComponent? { get }

    func draw()

    func addParent(par: VisualComponent)
    func removeParent()

    func depth() -> Int
    func count(color: String) -> Int
    func cover() -> Rect
}

protocol VisualGroup: VisualComponent {
    var numChildren: Int { get }
    var children: [VisualComponent] { get }

    mutating func add(child: VisualComponent)
    mutating func remove(child: VisualComponent)
    mutating func removeChild(at: Int)
}

class VisualComponentClass: VisualComponent {
    var boundingBox: Rect
    var color: String
    init() {
        color = ""
        boundingBox = Rect()
    }
    
    var parent: VisualComponent?
    func addParent(par: VisualComponent) {
        parent = par
    }
    func removeParent() {
        parent = nil
    }

    func draw() { }

    func depth() -> Int {
        return 1
    }
    func count(color: String) -> Int {
        return self.color == color ? 1 : 0
    }
    func cover() -> Rect {
        return boundingBox
    }
}

class VisualGroupClass: VisualComponentClass, VisualGroup {
    var numChildren: Int
    var children: [VisualComponent]
    override init() {
        numChildren = 0;
        children = []
    }
    
    func add(child: VisualComponent) {
        children.append(child)
        numChildren += 1

        child.addParent(par: self)
        update()
    }
    
    func remove(child: VisualComponent) {
        var index : Int?
        for (id, curr) in children.enumerated() {
            if (compareComp(a: curr, b: child)) {
                index = id
                break
            }
        }
        if let index = index {
            children[index].removeParent();
            children.remove(at: index)
        }

        update()
    }
    
    func removeChild(at: Int) {
        children[at].removeParent();
        children.remove(at: at)
        numChildren -= 1

        update()
    }

    func update() { }

    override func depth() -> Int {
        var mx = 0
        for child in children {
            mx = max(mx, child.depth())
        }

        return mx + 1
    }

    override func count(color: String) -> Int {
        var cnt = 0
        for child in children {
            cnt += child.count(color: color)
        }

        return cnt + (self.color == color ? 1 : 0)
    }
}

func compareComp(a: VisualComponent, b: VisualComponent) -> Bool {
    if (a.boundingBox == b.boundingBox && a.color == b.color) {
        return true
    }
    return false
} 

struct Point {
    var x: Double
    var y: Double

    init (x: Double, y: Double) {
        self.x = x;
        self.y = y;
    }

    static func == (this: Point, other: Point) -> Bool {
        if (this.x == other.x && this.y == other.y) {
            return true
        }
        return false
    }
}

struct Rect {
    //top-left
    var top: Point
    var width: Double
    var height: Double
    
    func printingString() -> String {
        return "x: \(top.x), y: \(top.y), width: \(width), height: \(height)";
    }

    init(x: Double, y: Double, width: Double, height: Double) {
        top = Point(x: x, y: y)
        self.width = width
        self.height = height
    }

    init() {
        top = Point(x: 0, y: 0)
        width = 0
        height = 0 
    }

    static func == (this: Rect, other: Rect) -> Bool {
        if (this.top == other.top && this.width == other.width && this.height == other.height) {
            return true
        }
        return false
    }
}

class Triangle: VisualComponentClass {
    override func draw() {
        print("Triangle(\(boundingBox.printingString()), color: \(color))")
    }
    
    init(a: Point, b: Point, c: Point, color: String) {
        super.init()

        let up = max(a.y, b.y, c.y)
        let down = min(a.y, b.y, c.y)
        let right = max(a.x, b.x, c.x)
        let left = min(a.x, b.x, c.x)

        boundingBox = Rect(x: left, y: up, width: right-left, height: up-down)
        
        self.color = color
    }
}

class Rectangle: VisualComponentClass {
    override func draw() {
        print("Rectangle(\(boundingBox.printingString()), color: \(color))")
    }
    
    init(x: Int, y: Int, width: Int, height: Int, color: String) {
        super.init()

        boundingBox = Rect(x: Double(x), y: Double(y), width: Double(width), height: Double(height))
        self.color = color
    }
}

class Circle: VisualComponentClass {
    override func draw() {
        print("Circle(\(boundingBox.printingString()), color: \(color))")
    }
    
    init(x: Int, y:Int, r: Double, color: String) {
        super.init()

        boundingBox = Rect(x: Double(x)-r, y: Double(y)+r, width: 2*r, height:2*r)
        self.color = color
    }
}

class Path: VisualComponentClass {
    override func draw() {
        print("Path(\(boundingBox.printingString()), color: \(color))")
    }
    
    init(points: [Point], color: String) {
        super.init()

        var up = points[0].y, down = points[0].y
        var right = points[0].x, left = points[0].x

        for curr in points {
            up = max(curr.y, up)
            down = min(curr.y, down)
            right = max(curr.x, right)
            left = min(curr.x, left)
        }

        boundingBox = Rect(x: left, y: up, width: right-left, height: up-down)
        
        self.color = color
    }
}

class HStack: VisualGroupClass {
    override func update() {
        var height = 0.0, width = 0.0

        for child in children {
            height = max(height, child.boundingBox.height)
            width += child.boundingBox.width
        }

        boundingBox.height = height
        boundingBox.width = width
    }
    
    override func draw() {
        print("Hstack(\(boundingBox.printingString()), color: \(color))")
    }
}

class VStack: VisualGroupClass {
    override func update() {
        var height = 0.0, width = 0.0

        for child in children {
            height += boundingBox.height
            width = max(width, child.boundingBox.width)
        }

        boundingBox.height = height
        boundingBox.width = width
    }
    
    override func draw() {
        print("Vstack(\(boundingBox.printingString()), color: \(color))")
    }
}


class ZStack: VisualGroupClass {
    override func update() {
        var up = children[0].boundingBox.top.y, down = children[0].boundingBox.top.y
        var right = children[0].boundingBox.top.x, left = children[0].boundingBox.top.x

        for curr in children {
            let currBox = curr.boundingBox

            up = max(currBox.top.y, up)
            down = min(currBox.top.y - currBox.height, down)
            left = min(currBox.top.x, left)
            right = max(currBox.top.x + currBox.width, right)
        }

        boundingBox.width = right - left
        boundingBox.height = up - down
    }

    override func draw() {
        print("Zstack(\(boundingBox.printingString()), color: \(color))")
    }
}

func depth(root: VisualComponent?) -> Int {
    if let curr = root {
        return curr.depth()
    } else {
        return 0
    }
}


func count(root: VisualComponent?, color: String) -> Int {
    if let curr = root {
        return curr.count(color: color)
    } else {
        return 0
    }
}

func cover(root: VisualComponent?) -> Rect {
    if let curr = root {
        return curr.cover()
    } else {
        return Rect()
    }
}


/*
var tri = Triangle(a: Point(x:1, y:1), b: Point(x:2, y:4), c: Point(x:8, y:-2), color: "blue")
tri.draw()

var rect = Rectangle(x: 1, y: 2, width: 3, height: 4, color: "red")
rect.draw()

var circ = Circle(x: 1, y: 1, r: 1, color: "green")
circ.draw()


var path = Path(points: [Point(x:2, y:4), Point(x:3, y:5), Point(x:4, y:2)], color: "gray")
path.draw()*/