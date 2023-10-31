import turtle
import os

# Set active directory to GUI folder:
os.chdir(os.path.dirname(os.path.abspath(__file__)))

# Create a turtle screen:
wn = turtle.Screen()
wn.bgcolor("white")  # Set the background color
wn.title("BWT Comps GUI")

# Create a Turtle object
t = turtle.Turtle()
t.speed(1)  # Set the drawing speed
t.hideturtle()


# Define the animation functions

slides = ["Table of Contents", "Intro", "Transform Explanation", "Decoding I", "Decoding II", "NGS Application", "NGS Example", "BWT + NGS Metrics", "Closing Thoughts"]
used_keys = ["C","1", "2", "3", "4", "5", "6", "7", "8"]

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

def bwt_quick(input_string):
    input_string += '$'
    indices = list(range(len(input_string)))
    sorted_indices = sorted(indices, key=lambda i: input_string[i:] + input_string[:i])
    bwt_encoded = ''.join(input_string[i - 1] for i in sorted_indices)
    return(bwt_encoded)

def titlescreen():
    title = "The Burrows Wheeler Transform in NGS"
    subtext = "A 2023 CS Comps GUI"
    subtext_2 = "(Press C for table of contents)"
    draw_header(f'{title}', -200, 0)
    draw_text(f'{subtext}', -110, -50)
    draw_text(f'{subtext_2}', -130, -300)

def contents():
    clear_turtle()
    header = "Table of Contents:"
    draw_header(f'{header}', -100, 200)
    
    draw_text('Slides:', -150, -25)
    draw_text('Key Input:', 10, -25)
    
    y = -60
    x1 = -150
    x2 = 50
    
    for i in range(len(slides)):
        # Slide title
        draw_text(slides[i], x1, y)

        # Slide 
        draw_text(used_keys[i], x2, y)

        y -= 30
               
                

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


def transform_visual(input_string):
    clear_turtle()
    codeword = bwt_quick(input_string)
    input_string = input_string + "$"
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
    clear_turtle()
    slide_paragraph=f'Now that we have our codeword: {input_string}, what if we want to decode it?\n'\
                    'This is where we introduce the mathematical properties of the Burrows-Wheeler Transform.\n'\
    
    draw_text(f'{slide_paragraph}', -300, 200)

    para_2 = 'The Burrows-Wheeler Transform has a unique property called "Last, First" ordering. \n'\
             'But what does this mean? Well, this is best demonstrated visually:'
    
    draw_text(f'{para_2}', -300, 150)

    input = 'BANANA$'
    inp = [*input]
    inp2 = [*input_string]

    x = - 100
    y = 10
    x2 = 100

    for i in range(len(input)):
        draw_text(f'{inp[i]}', x, y)
        draw_text(f'{inp2[i]}', x2, y)
        y -= 20
    
    para_3 = 'To begin, Last, First ordering (LF) essentially means that the order in which characters appear \n'\
             f'in the codeword {input_string}, is the same order in which they appear in the original \n'\
             f'string, {input}. See below numeration to clarify:'
    
    draw_text(f'{para_3}', -300, 75)
    
    original_numbers = [1, 1, 1, 2, 2, 3, 1]
    transform_numbers = [1, 1, 2, 1, 1, 2, 3]

    x = - 130
    y = 10
    x2 = 70

    for i in range(len(input)):
        draw_text(f'{original_numbers[i]}', x, y)
        draw_text(f'{transform_numbers[i]}', x2, y)
        y -= 20
    pass

    para_4 = 'The practical decoding method will be explained on the following slide. However, L-F mapping \n'\
             'is the foundation for more time and space efficient decoding methodology, such as the FM \n'\
             'index, which you can read about in the Mathematics section of our group paper.'
    
    draw_text(f'{para_4}', -300, -260)

def bwt_decode(input):

    # Building the decoding table

    bwt_encoded = bwt_quick(input)
    table = [''] * len(bwt_encoded)
    
    for _ in range(len(bwt_encoded)):
        new_table = []
        for i in range(len(bwt_encoded)):
            new_table.append(bwt_encoded[i] + table[i])
        
        table = sorted(new_table)
    
    # original_string = [s for s in table if s.endswith('$')][0]
    
    return table
        
        
def decoding_II(input_string):
   
   clear_turtle()
   
   keyword = bwt_quick(input_string)
   
   slide_paragraph = 'The practical decoding process for the BWT makes use of another of its remarkable encoding qualities,\n'\
                     f'namely, that you can reconstruct the complete encoding table using just our codeword, {bwt_quick(keyword)}.\n'\
                     '\n'\
                     'This is accomplished using a simple three-step algorithm: \n'

   draw_text(f'{slide_paragraph}', -300, 200)

   step_1 = '  Begin with a \n'\
            'lexicographically \n'\
            ' sorted keyword:'

   draw_text(f'{step_1}', -360, 100)

   display_keyword = list(keyword)

   keyword_unsorted = list(keyword)

   display_keyword.sort()

   x1 = -310
   y1 = 50

   for i in range(len(display_keyword)):
       draw_text(display_keyword[i], x1, y1)
       y1 -= 20

   step_2 = '1. Add another keyword to \n'\
            'the sorted keyword, adding a\n'\
            'character to each index:'
   
   draw_text(f'{step_2}', -190, 100)

   x2 = -140
   y2 = 50
   
   for i in range(len(display_keyword)):
       display_keyword[i] += keyword_unsorted[i]
       draw_text(display_keyword[i], x2, y2)
       y2 -= 20
    
   display_keyword.sort()

   step_3 = '2. Sort the new strings:'

   draw_text(f'{step_3}', 55, 100)

   x3 = 140
   y3 = 50

   for i in range(len(display_keyword)):
       draw_text(display_keyword[i], x3, y3)
       y3 -= 20

   matrix = bwt_decode(input_string)

   step_4 = '3. Repeat steps 1 + 2 \n'\
            '   one time for each \n' \
            '   character in the \n'\
            '   original string: \n'
   
   draw_text(f'{step_4}', 290, 100)

   x4 = 310
   y4 = 50

   for i in matrix:
       draw_text(i, x4, y4)
       y4 -= 20

   end_paragraph = 'Using this decoding algorithm, we are guaranteed to end up with a string ending in the sentinel character, $. \n'\
                   'The final step in decoding is to return the string ending in $. If decoded properly, this string will always be our  \n'\
                   'original input string we encoded!'
   
   draw_text("<<<", 410, -30)

   draw_text(f'{end_paragraph}', -300, -200)

def ngs_I(input_string):
    # Explanations required:

    clear_turtle()

    para_1 = 'Now that we have gone over the mechanics of the BWT, we can begin the discussion of the BWT as a tool in genomics. \n'\
             '\n'\
             'The BWT was originally intented to be used with text-compression software. Using the encoding method discussed on previous \n'\
             f'slides, the BWT creates runs of repeated characters. Using our input string, {input_string}, we see that it becomes:'
    
    output_string = bwt_quick(input_string)

    draw_text(f'{para_1}', -400, 200)

    draw_text(f'{output_string}', -50, 150)

    para_2 = 'Using text-compression algorithms such as RLE (Run-Length-Encoding), as the BWT was intended for, the transformed string becomes:'

    compressed_string = '1A2N1$1B2A'

    draw_text(f'{para_2}', -400, 100)

    draw_text(f'{compressed_string}', -50, 50)

    para_3 = 'Which, as it happens, is actually a longer string that our codeword. However, consider applying this a genome, a \n'\
             'text consisting of only four character: A, C, G & T, being the protein bases of DNA, and, at the time of this project, \n'\
             'with the smallest discovered genome length being 159,662 characters (Carsonella rudii). Take the following base-pair sequence \n'\
             'for instance:'
    
    string_sequence = 'ACATGAGCATCATCACGTACTATCAGCTATCGTATCGATCTGAGCTATG'

    sequence_display = 'ACATGAGC \n'\
                       'ATCATCAC \n'\
                       'GTACTATC \n'\
                       'AGCTATCG \n'\
                       'TATCGATC \n'\
                       'TGAGCTATG'

    draw_text(f'{para_3}', -400, -50)

    draw_text(f'{sequence_display}', -400, -200)

    inter_1 = '==>  Applied BWT ==>'

    draw_text(f'{inter_1}', -350, -250)


    string_display= 'G$CTGCGC \n'\
                      'TCTTGTCT \n'\
                      'TTGATATA \n'\
                      'GGTTTTCA \n'\
                      'AACCGCGC \n' \
                      'CAAAAAAA \n'\
                      'AC'

    draw_text(f'{string_display}', -200, -200)

    inter_2 = '==>  Applied RLE Compression ==>'

    draw_text(f'{inter_2}', -150, -250)

    # G$CTGCGCTCTTGTCTTTGATATAGGTTTTCAAACCGCGCCAAAAAAAAC

    string_compressed = 'G$CTGCGCTCT2TGTCT3GATATAG2T4CA3CGCGCC7A3C'
    
    compressed_display = 'G$CTGCGC \n' \
                        'TCT2TGTC \n' \
                        'T3GATATA \n' \
                        'G2T4CA3C \n' \
                        'GCGCC7A3C'
    
    draw_text(f'{compressed_display}', 0, -200)

    diff_len = len(string_sequence) - len(string_compressed)

    inter_3 = 'Total difference in string length:'

    draw_text(f'{inter_3}', 200, -130)

    draw_text(f'{diff_len}', 300, -160)

    para_4 = 'Given the string length, 8 characters saved may seem trivial, however—consider the size of a genome. \n'\
             'At a minimum of over 150,000 base pairs, the employment of the BWT has revolutionized how genomes are \n'\
             'stored, shared and sequenced, allowing geneticists to push the boundaries of modern genomics to new extremes.'
    
    draw_text(f'{para_4}', -400, -350)

def ngs_II():
    clear_turtle()
   
    para_1 = 'Now that we have an appropriate context for the use of the BWT in genomics, we can look at \n' \
             'the how the NGS (Next Generational Sequencing) process utilizes BWT encoding. But first, what is NGS?'\
    
    para_2 = 'NGS is a process whereby fragmented half-DNA strands are matched to a position (or multiple positions) \n' \
             'on an existing DNA model (See Biology in paper lit. review). This is the process by which genomes are mapped \n.'\
             'For example, given the model and sequence:'
    
    draw_text(f'{para_1}', -400, 200)

    draw_text(f'{para_2}', -400, 120)

    # Need DNA String (could be random, just a model)

    model = 'ACTGACTGTCATCGTACGTAGCTACGACGTAGCATGCGACGTAGCACGACGAGTCGACGTAGCTAGCGACGA'
    #                           AGCTACGACGT #Aligning this manually is gonna be a chore

    substring = 'AGCTACGACGT'

    draw_text(f'{model}', -400, 50)
    draw_text(f'{substring}', -50, 10)

    inter_1 = 'Feeding both the model and the sequence into an alignment tool, the substring is mapped to the correct location in the model:'

    draw_text(f'{inter_1}', -400, -50)



    mapped_substring = 'ACTGACTGTCATCGTACGTAGCTACGACGTAGCATGCGACGTAGCACGACGAGTCGACGTAGCTAGCGACGA \n' \
                       #'                  ^^^^^^^^^^^ \n'\
                      # '                  AGCTACGACGT' # What are the odds this works
    
    draw_text(f'{mapped_substring}', -400, -120)
    draw_text(f'{substring} located at range [20, 30] (1-indexed)', -195, -150)
    

    para_3 = 'As briefly mentioned on slide three, the involement of the BWT is based on an opportunistic data structures \n'\
             'called the FM-index. Built by Farragina and Manzini, the index is a set of two tables whose construction takes linear time \n'\
             '(i.e. a larger input has a proportionally longer build time), but functions sublinearly, meaning input scaling favors large inputs. \n'\
             'In non-CS terminology, the FM index essentially makes it far FAR more efficient to align sequences to a large model. \n'\
             'If you would like to learn more about how this is accomplished, see the Math and Algorithm sections of our paper.'
    
    draw_text(f'{para_3}', -400, -300)

def ngs_III():
    # Talk about how BWT revolutionized genomics. Might import images here? Figures from paper?
    clear_turtle()
    
    para_1 = 'The application of the BWT to short-read sequence aligners took off in the late 2000s, with development of Bowtie, \n'\
             'in 2008 followed closely behind by BWA (Burrows Wheeler Aligner) and SOAP2 in 2009. Both tools demonstrated that BWT-based aligners \n'\
             'represented the next evolution of sequence alignment. For tangible metrics, see the below figure (citation included) where all three \n'\
             'are compared against MAQ, the previously best short-read aligned on the market:'
    
    draw_text(f'{para_1}', -400, 200)

    citation = 'Li, Heng, and Richard Durbin. “Fast and Accurate Short Read Alignment with Burrows-Wheeler Transform.” \n' \
               'Bioinformatics, vol. 25, no. 14, July 2009, pp. 1754-60. Silverchair, \n' \
               'https://doi.org/10.1093/bioinformatics/btp324.'
    
    wn.addshape('resized_figure.gif')
    image = turtle.Turtle()
    image.pencolor("white")
    image.goto(0, -100)
    image.shape('resized_figure.gif')

    draw_text(f'{citation}', -400, -430)

    note_1 = '1. Processing time: \n' \
             'Notice the drastic difference \n'\
             'In processing speeds. All three \n'\
             'BWT-based aligners were multiple \n'\
             'orders of magnitued faster.'
    
    draw_text(f'{note_1}', -470, -25)
    
    note_2 = '2. Confidence: \n' \
             'In single-ended pairings, \n' \
             'the BWT aligners scored lower \n' \
             'than mac for all read lengths. \n' \
             'One the other hand, BWA had \n ' \
             'a consistently higher confidence \n'\
             'confidence score in Pair-ended \n' \
             'alignment.'

    draw_text(f'{note_2}', -470, -200)

    note_3 = '3. Error: \n' \
             'Granted two exceptions, \n' \
             'the BWT aligners had higher \n' \
             'error across the board'
    
    draw_text(f'{note_3}', 230, -100)

def conclusion():
    clear_turtle()
    # Statement of purpose of this GUI—Overview, not in-depth explanations
    # Reference paper again
    # Reference our own CLI available for use/download
    # Brief talk about the ethics/consequences of BWT making genomics more accessible
    pass

def main():
    input_string = "BANANA"
    wn.listen()
    titlescreen()

    wn._root.attributes('-topmost', 1)
    wn._root.attributes('-topmost', 0)

    wn.onkey(lambda: contents(), "c")
    wn.onkey(lambda: bwt_intro(input_string), "1")
    wn.onkey(lambda: transform_visual(input_string), "2")
    wn.onkey(lambda: bwt_decoding(bwt_quick(input_string)), "3")
    wn.onkey(lambda: decoding_II('BANANA'), "4")
    wn.onkey(lambda: ngs_I(input_string), "5")
    wn.onkey(lambda: ngs_II(), "6")
    wn.onkey(lambda: ngs_III(), "7")
    wn.onkey(lambda: conclusion(), "8")
    wn.onkey(lambda: wn.bye(), "q")  # Exit the program on 'q' key press
    wn.mainloop()

if __name__ == '__main__':
    main()
