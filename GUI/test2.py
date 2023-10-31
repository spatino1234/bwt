import turtle
import tkinter as tk
from PIL import Image, ImageTk
import os
os.chdir(os.path.dirname(os.path.abspath(__file__)))

# Create a Turtle graphics window
window = turtle.Screen()

# Create a Turtle object for geometric shapes
t = turtle.Turtle()
t.shape("circle")  # You can use other built-in shapes

# Set the turtle's position
t.penup()
t.goto(-100, 0)

# Create a tkinter window for the image overlay
root = tk.Tk()
root.geometry("400x400")

# Load the image using PIL (Pillow)
image = Image.open('figure.png')

# Convert the image to PhotoImage
image_tk = ImageTk.PhotoImage(image)

# Create a label for the image
label = tk.Label(root, image=image_tk)
label.pack()

# Keep the tkinter window open
root.mainloop()

# Keep the turtle graphics window open
turtle.done()





