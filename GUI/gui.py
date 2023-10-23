import turtle

def draw_text(text, x, y):
    t.penup()
    t.goto(x, y)
    t.pendown()
    t.write(text, font=("Arial", 16, "normal"))

# Initialize the turtle
wn = turtle.Screen()
t = turtle.Turtle()
t.speed(1)
t.hideturtle()

# BWT input string
input_string = turtle.textinput("Input", "Enter a string:")

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

# Close the turtle graphics window on click
wn.exitonclick()
