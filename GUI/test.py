import turtle

# Create a turtle screen
wn = turtle.Screen()
wn.bgcolor("white")  # Set the background color
wn.title("BWT Comps GUI")

# Create a Turtle object
t = turtle.Turtle()
t.speed(1)  # Set the drawing speed
t.hideturtle()

# Define the animation functions

def bwt_quick(input_string):
    input_string += '$'
    indices = list(range(len(input_string)))
    sorted_indices = sorted(indices, key=lambda i: input_string[i:] + input_string[:i])
    bwt_encoded = ''.join(input_string[i - 1] for i in sorted_indices)
    return(bwt_encoded)

def bwt_intro(input_string):
    clear_turtle()
    slide_paragraph='Welcome to the BWT 2023 comps group Graphical User Interface. \n'\
                    'We have developed this simple python animation to demonstrate the encoding, \n'\
                    'functionality, and application of the Burrows-Wheeler Transform in Short-Read alignment. \n'\
                    'Below is our example string and corresponding BWT codeword. \n'\
    

    draw_text(f'{slide_paragraph}', -300, 200)
    bwt_encoded = bwt_quick(input_string)
    # Visualization
    draw_text("Original String:", -200, 100)
    draw_text(input_string, -100, 70)

    draw_text("BWT Encoded String:", -200, 0)
    draw_text(bwt_encoded, -100, -30)

    wn._root.attributes('-topmost', 1)
    wn._root.attributes('-topmost', 0)


def clear_turtle():
    t.clear()  # Clear the turtle's drawings

def draw_text(text, x, y):
    t.penup()
    t.goto(x, y)
    t.pendown()
    t.write(text, font=("Arial", 16, "normal"))

def transform_visual(input_string):
    clear_turtle()
    codeword = bwt_quick(input_string)
    input_string = input_string + "$"
    clear_turtle()
    slide_paragraph='The first step in employing the BWT is to encode our codeword. This happens in four steps: \n'\
                    '1. We add a sentinel character, $, to the end of the string. \n'\
                    '2. We then build a table of cyclically rotated (permuting characters one to the right) transformations. \n'\
                    '3. Next, We lexicographically order the cyclical transformations \n'\
                    '4. Finally, we take the final column of our lexicographically ordered transformations to get the codeword. \n'\
                    
    draw_text(f'{slide_paragraph}', -400, 200)
    draw_text("1. Our input (with added sentinel character):", -200, 100)
    draw_text(input_string, -100, 70)
    rotations = cyclic_transformations(input_string)

    draw_text("2. Cyclic rotations:", -200, 0)
    y = -30
    for i in rotations:
        draw_text(f'{i}', -100, y)
        y -= 30
    
    draw_text("3. Lexicographically ordered rotations:", 0, 0)
    rotations.sort()
    y = -30
    for i in rotations:
        draw_text(f'{i}', 100, y)
        y -= 30
     
    draw_text("4. Final column of lexicographically ordered rotations:", -200, -270)
    draw_text(f'{codeword}', -100, -300)

    wn._root.attributes('-topmost', 1)
    wn._root.attributes('-topmost', 0)


# Visual transformation helper methods:

def cyclic_transformations(input_string):
    cyclic_transforms = []

    # Iterate through the string and create cyclic transformations
    for i in range(len(input_string)):
        cyclic_transform = input_string[i:] + input_string[:i]
        cyclic_transforms.append(cyclic_transform)

    return cyclic_transforms

# Initialize the turtle

def bwt_decoding(input_string):
    wn.clear()
    slide_paragraph=f'Now that we have our codeword: {input_string}, what if we want to decode it?\n'\
                    'This is where we introduce the mathematical properties of the Burrows-Wheeler Transform.\n'\
                    'First, I did not finish this part. My fault. \n'\
                    ' \n'\
                    '\n'\
    
    draw_text(f'{slide_paragraph}', -300, 200)

    wn._root.attributes('-topmost', 1)
    wn._root.attributes('-topmost', 0)

    pass

def main():
    input_string = "BANANA"
    wn.listen()

    wn._root.attributes('-topmost', 1)
    wn._root.attributes('-topmost', 0)

    wn.onkey(lambda: bwt_intro(input_string), "1")
    wn.onkey(lambda: transform_visual(input_string), "2")
    wn.onkey(lambda: bwt_decoding(input_string), "3")
    wn.onkey(lambda: wn.bye(), "q")  # Exit the program on 'q' key press
    wn.mainloop()

if __name__ == '__main__':
    main()
