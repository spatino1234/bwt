# The test file where A.J. has all of his mental breakdowns
import turtle
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import PhotoImage
import os
os.chdir(os.path.dirname(os.path.abspath(__file__)))

# Create a turtle screen
wn = turtle.Screen()
wn.bgcolor("white")  # Set the background color
wn.title("BWT Comps GUI")

# Create a Turtle object
t = turtle.Turtle()
# Set the drawing speed
t.hideturtle()

#1160 × 1230
original_image = Image.open('figure.gif')
resized_image = original_image.resize((450, 510), Image.ANTIALIAS)
resized_image.save('resized_figure.gif')

def bwt_quick(input_string):
    input_string += '$'
    indices = list(range(len(input_string)))
    sorted_indices = sorted(indices, key=lambda i: input_string[i:] + input_string[:i])
    bwt_encoded = ''.join(input_string[i - 1] for i in sorted_indices)
    return(bwt_encoded)

def clear_turtle():
    t.clear()  # Clear the turtle's drawings

def draw_text(text, x, y):
    t.penup()
    t.goto(x, y)
    t.pendown()
    t.write(text, font=("Times New Roman", 16, "normal"))

def draw_header(text, x, y):
    t.penup()
    t.goto(x, y)
    t.pendown()
    t.write(text, font=("Times New Roman", 20, "normal"))

def conclusion():
    clear_turtle()

    header ='Concluding Thoughts'
    draw_header(f'{header}', -100, 300)

    para_1 = 'In truth, this GUI is the proverbial tip of the iceberg for this project. The research, planning, \n'\
             'effort, and expertise of six individual group members (and one comps advisor) went into making this \n'\
             'project a reality. Every member of this team has left their mark on this project, and this GUI does not \n'\
             'do that justice. Because of this, we implore you to further investigate all resources associated with this project. \n'
    
    draw_text(f'{para_1}', -325, 175)
    
    para_2 = 'For further discussion of literature, technology, algorithms, methods and ethics, \n' \
             'see our paper, "Short-Read Sequence Alignment with the Burrows-Wheeler Transform."'
    
    draw_text(f'{para_2}', -275, 100)
    
    para_3 = 'For our own C-based implementation of a BWT-based short-read aligner, see our linked github.'
    
    draw_text(f'{para_3}', -275, 0)

    para_4 = 'Lastly, we, the BWT comps group, would like to offer special thanks to Professor Layla Oesper for \n'\
             'supporting us throughout this project; and also the CS department for helping us get this far.'
    
    draw_text(f'{para_4}', -275, -150)

    para_5 = 'Sincerely, \n'\
             ' \n '\
             '  - Shreya Nair, Phi Rapacz, Serafín Patiño, Zeb Goldhaber-Gordon, Jack Owens, and A.J. Ristino \n' 
    
    draw_text(f'{para_5}', -275, -240)



def main():
    input_string = "BANANA"
    wn.listen()

    wn._root.attributes('-topmost', 1)
    wn._root.attributes('-topmost', 0)

    conclusion()
    # wn.onkey(lambda: ngs_I(input_string), "5")
    wn.onkey(lambda: wn.bye(), "q")  # Exit the program on 'q' key press
    wn.mainloop()


    

if __name__ == '__main__':
    main()
