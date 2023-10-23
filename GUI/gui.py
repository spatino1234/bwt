import turtle

def draw_text(text, x, y):
    t.penup()
    t.goto(x, y)
    t.pendown()
    t.write(text, font=("Arial", 16, "normal"))

def draw_arrow(x1, y1, x2, y2):
    t.penup()
    t.goto(x1, y1)
    t.pendown()
    t.setheading(t.towards(x2, y2))
    t.forward(t.distance(x2, y2))
    t.penup()
    t.goto(x2, y2)
    t.pendown()
    t.setheading(t.towards(x1, y1))
    t.forward(t.distance(x1, y1))

# Initialize the turtle
wn = turtle.Screen()
t = turtle.Turtle()
t.speed(1)
t.hideturtle()

# BWT input string
input_string = "ACATGAGCAT"

# Perform BWT
input_string += '$'
indices = list(range(len(input_string)))
sorted_indices = sorted(indices, key=lambda i: input_string[i:] + input_string[:i])
bwt_encoded = ''.join(input_string[i - 1] for i in sorted_indices)

# Visualization
draw_text("Original String:", -200, 100)
draw_text(input_string, -100, 70)

draw_text("BWT Encoded String:", -200, 0)
draw_text(bwt_encoded, -100, -30)

# draw_arrow(-100, 50, -100, 10)

# Close the turtle graphics window on click
wn.exitonclick()
