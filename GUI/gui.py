import turtle

wn = turtle.Screen()
t = turtle.Turtle()
t.speed(1)
t.hideturtle()

functions = []

def clear_turtle():
    wn.clear()

def draw_text(text, x, y):
    t.penup()
    t.goto(x,y)
    t.pendown()
    t.write(text, font=("Arial", 16, "normal"))

# Opening slide

def bwt_quick(input_string):
    input_string += '$'
    indices = list(range(len(input_string)))
    sorted_indices = sorted(indices, key=lambda i: input_string[i:] + input_string[:i])
    bwt_encoded = ''.join(input_string[i - 1] for i in sorted_indices)
    return(bwt_encoded)


def bwt_intro(input_string):
    # Perform BWT
   
    bwt_encoded = bwt_quick(input_string)
    # Visualization
    draw_text("Original String:", -200, 100)
    draw_text(input_string, -100, 70)

    draw_text("BWT Encoded String:", -200, 0)
    draw_text(bwt_encoded, -100, -30)
    
# Visual transformation:

def transform_visual(input_string):
    input_string = input_string + "$"
    clear_turtle()
    draw_text("Your input (with added sentinel character):", -200, 100)
    draw_text(input_string, -100, 70)
    rotations = cyclic_transformations(input_string)

    draw_text("Cyclic rotations:", -200, 0)
    y = -30
    for i in rotations:
        draw_text(f'{i}', -100, y)
        y -= 30
    
    draw_text("Lexicographically ordered rotations:", 0, 0)
    rotations.sort()
    y = -30
    for i in rotations:
        draw_text(f'{i}', 50, y)
        y -= 30
    
    # draw_text 

# Visual transformation helper methods:

def cyclic_transformations(input_string):
    cyclic_transforms = []

    # Iterate through the string and create cyclic transformations
    for i in range(len(input_string)):
        cyclic_transform = input_string[i:] + input_string[:i]
        cyclic_transforms.append(cyclic_transform)

    return cyclic_transforms

# Initialize the turtle

def main():
    input_string = turtle.textinput("Input", "Enter a string:")
    bwt_intro(input_string)
    wn.onscreenclick(lambda x, y: transform_visual(input_string))
    turtle.mainloop()

if __name__ == '__main__':
    # This code won't run if this file is imported.
    main()


