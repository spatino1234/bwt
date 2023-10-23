import turtle

''' TODO: current version of code only shows the final rotated word
        right before it becomes the original word.
'''

def draw_string(s):
    # Clear previous drawing
    turtle.clear()
    
    # Set position
    turtle.penup()
    turtle.goto(-50, 0)
    turtle.pendown()

    # Write string
    turtle.write(s, font=("Arial", 20, "normal"))

    # Pause for a bit
    turtle.delay(500)


def rotate_string(s):
    # add last char then the rest of the letters
    return s[-1] + s[:-1]


def main():
    # Set up the turtle environment
    turtle.speed(0)
    turtle.bgcolor("white")
    turtle.hideturtle()
    # Turn off animation for immediate update
    turtle.Screen().tracer(0)  

    input_string = turtle.textinput("Input", "Enter a string:")
    
    if input_string:
        current_rotation = input_string

        while True:
            draw_string(current_rotation)
            current_rotation = rotate_string(current_rotation)
            if current_rotation == input_string:
                break

        turtle.done()


if __name__ == "__main__":
    main()
