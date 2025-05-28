import sys

# Does not work

class Vec2D:
    EPS = 1e-9 

    x: float

    y: float

    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y
    def __sub__(self, other: "Vec2D") -> "Vec2D":
        return Vec2D(self.x - other.x, self.y - other.y)
    def __add__(self, other: "Vec2D") -> "Vec2D":
        return Vec2D(self.x + other.x, self.y + other.y)
    def __mul__(self, scalar: float) -> "Vec2D":
        return Vec2D(self.x * scalar, self.y * scalar)
    def __div__(self, scalar: float) -> "Vec2D":
        return Vec2D(self.x / scalar, self.y / scalar)
    def __eq__(self, other: "Vec2D") -> bool:
        return abs(self.x - other.x) < self.EPS and abs(self.y - other.y) < self.EPS

    def cross(self, other: "Vec2D") -> float:
        return self.x * other.y - self.y * other.x
    def dot(self, other: "Vec2D") -> float:
        return self.x * other.x + self.y * other.y
    def dist(self, other: "Vec2D") -> float:
        return ((self.x - other.x) ** 2 + (self.y - other.y) ** 2) ** 0.5

Point = Vec2D
Segment = tuple[Point, Point]

def intersect_col(s1: Segment, s2: Segment) -> list[Point]:
    u = s1[0] - s2[0]
    v = s1[0] - s1[1]

    # Lines are parallel
    if abs(v.cross(u)) > Vec2D.EPS:
        return []

    in_between = lambda a, b, x : a <= x <= b or b <= x <= a
    
    if not in_between(s1[0].x, s1[1].x, s2[0].x) and \
        not in_between(s1[0].x, s1[1].x, s2[1].x) and \
        not in_between(s2[0].x, s2[1].x, s1[0].x) and \
        not in_between(s2[0].x, s2[1].x, s1[1].x):
        return []

    res = [s1[0], s1[1], s2[0], s2[1]]
    res.sort(key=lambda p: p.x)

    res = res[1:]
    res = res[:-1]

    if res[0].dist(res[1]) < Vec2D.EPS:
        return [res[0]]

    return res

def intersect(s1: Segment, s2: Segment) -> list[Point]:
    p = s1[0]
    r = s1[1] - s1[0]

    q = s2[0]
    s = s2[1] - s2[0]

    rXs = r.cross(s)
    if abs(rXs) < Vec2D.EPS:
        return intersect_col(s1, s2)

    pq = q - p
    t = pq.cross(s) / rXs
    u = pq.cross(r) / rXs

    if not (0 <= t <= 1 and 0 <= u <= 1):
        return []

    res = p + r * t
    return [res]

if __name__ == "__main__":
    n = int(sys.stdin.readline().strip())
    for _ in range(n):
        x1, y1, x2, y2, x3, y3, x4, y4 = map(float, sys.stdin.readline().strip().split())

        s1 = (Point(x1, y1), Point(x2, y2))
        s2 = (Point(x3, y3), Point(x4, y4))

        res = intersect(s1, s2)
        if not res:
            print("none")
            continue

        if len(res) == 1:
            print(f"{res[0].x:.2f} {res[0].y:.2f}")
            continue

        print(f"{res[0].x:.2f} {res[0].y:.2f} {res[1].x:.2f} {res[1].y:.2f}")
