import pygame
import serial

# Define some colors.
BLACK = pygame.Color('black')
WHITE = pygame.Color('white')

# Open Serial port
# arduino = serial.Serial('/dev/tty.usbmodem143201', 19200)
arduino = serial.Serial('/dev/cu.usbserial-DN04P00J', 19200)

# This is a simple class that will help us print to the screen.
# It has nothing to do with the joysticks, just outputting the
# information.
class TextPrint(object):
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def tprint(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, (self.x, self.y))
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10


pygame.init()

# Set the width and height of the screen (width, height).
screen = pygame.display.set_mode((500, 700))

pygame.display.set_caption("My Game")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates.
clock = pygame.time.Clock()

# Initialize the joysticks.
pygame.joystick.init()

# Get ready to print.
textPrint = TextPrint()

# -------- Main Program Loop -----------
while not done:
    #
    # EVENT PROCESSING STEP
    #
    # Possible joystick actions: JOYAXISMOTION, JOYBALLMOTION, JOYBUTTONDOWN,
    # JOYBUTTONUP, JOYHATMOTION
    for event in pygame.event.get(): # User did something.
        if event.type == pygame.QUIT: # If user clicked close.
            done = True # Flag that we are done so we exit this loop.
        #elif event.type == pygame.JOYBUTTONDOWN:
            #print("Joystick button pressed.")
        #elif event.type == pygame.JOYBUTTONUP:
            #print("Joystick button released.")

    #
    # DRAWING STEP
    #
    # First, clear the screen to white. Don't put other drawing commands
    # above this, or they will be erased with this command.
    screen.fill(WHITE)
    textPrint.reset()

    # Get count of joysticks.
    joystick_count = pygame.joystick.get_count()

    textPrint.tprint(screen, "Number of joysticks: {}".format(joystick_count))
    textPrint.indent()

    # For each joystick:
    for i in range(joystick_count):


        joystick = pygame.joystick.Joystick(i)
        joystick.init()

        textPrint.tprint(screen, "Joystick {}".format(i))
        textPrint.indent()

        # Get the name from the OS for the controller/joystick.
        name = joystick.get_name()
        textPrint.tprint(screen, "Joystick name: {}".format(name))

        # Usually axis run in pairs, up/down for one, and left/right for
        # the other.
        axes = joystick.get_numaxes()
        textPrint.tprint(screen, "Number of axes: {}".format(axes))
        textPrint.indent()

        # Left Joystick X axis
        axis = joystick.get_axis(0)
        textPrint.tprint(screen, "{} Axis LJoyX value: {:>6.3f}".format(0, axis))
        value = 0
        # print(axis)
        if axis < 0:
            thrust = -0.2
            for x in range(1, 6):
                if axis < thrust:
                    value = x
                    x = 6
                else:
                    thrust -= 0.2
            print("arduino writing")
            arduino.write("LS-" + str(value) + "\n")
            print("LS-" + str(value) + "\n")

        if axis > 0:
            thrust = 0.2
            for x in range(1, 6):
                if axis > thrust:
                    value = x
                    x = 6
                else:
                    thrust += 0.2
            arduino.write("RS-" + str(value))

            if arduino.in_waiting:
                print(arduino.read())

        # Left Joystick Y axis
        axis = joystick.get_axis(1)
        textPrint.tprint(screen, "{} Axis LJoyY value: {:>6.3f}".format(1, axis))
        #arduino.write("RS")

        # Left Trigger, value from -1 to 1
        axis = joystick.get_axis(2)
        textPrint.tprint(screen, "{} Axis LT value: {:>6.3f}".format(2, axis))

        # Right Joystick X axis
        axis = joystick.get_axis(3)
        textPrint.tprint(screen, "{} Axis RJoyX value: {:>6.3f}".format(3, axis))
        #arduino.write("RY")

        # Right Joystick Y axis
        axis = joystick.get_axis(4)
        textPrint.tprint(screen, "{} Axis RJoyY value: {:>6.3f}".format(4, axis))
        #arduino.write("LY")

        # Right Trigger, value from -1 to 1
        axis = joystick.get_axis(5)
        textPrint.tprint(screen, "{} Axis RT value: {:>6.3f}".format(5, axis))

        textPrint.unindent()

        # Buttons
        buttons = joystick.get_numbuttons()
        textPrint.tprint(screen, "Number of buttons: {}".format(buttons))
        textPrint.indent()

        button = joystick.get_button(0)
        textPrint.tprint(screen,
                         "{:>2} Button A value: {}".format(0, button))
        if button == 1:
            arduino.write("UH-3")
        if arduino.in_waiting:
            print(arduino.read())

        button = joystick.get_button(1)
        textPrint.tprint(screen,
                         "{:>2} Button B value: {}".format(1, button))
        if button == 1:
            arduino.write("DH-3")
        if arduino.in_waiting:
            print(arduino.read())

        button = joystick.get_button(2)
        textPrint.tprint(screen,
                         "{:>2} Button X value: {}".format(2, button))

        button = joystick.get_button(3)
        textPrint.tprint(screen,
                         "{:>2} Button Y value: {}".format(3, button))

        button = joystick.get_button(4)
        textPrint.tprint(screen,
                         "{:>2} Button LB value: {}".format(4, button))

        button = joystick.get_button(5)
        textPrint.tprint(screen,
                         "{:>2} Button RB value: {}".format(5, button))

        # Left Joystick Button
        button = joystick.get_button(6)
        textPrint.tprint(screen,
                         "{:>2} Button LJoyB value: {}".format(6, button))

        # Right Joystick Button
        button = joystick.get_button(7)
        textPrint.tprint(screen,
                         "{:>2} Button RJoyB value: {}".format(7, button))

        button = joystick.get_button(8)
        textPrint.tprint(screen,
                         "{:>2} Button Start value: {}".format(8, button))

        button = joystick.get_button(9)
        textPrint.tprint(screen,
                         "{:>2} Button Back value: {}".format(9, button))

        button = joystick.get_button(10)
        textPrint.tprint(screen,
                         "{:>2} Button XBOX value: {}".format(10, button))

        button = joystick.get_button(11)
        textPrint.tprint(screen,
                         "{:>2} Button UpPad value: {}".format(11, button))

        button = joystick.get_button(12)
        textPrint.tprint(screen,
                         "{:>2} Button DownPad value: {}".format(12, button))

        button = joystick.get_button(13)
        textPrint.tprint(screen,
                         "{:>2} Button LeftPad value: {}".format(13, button))
        if button == 1:
            arduino.write("RL-4")

        button = joystick.get_button(14)
        textPrint.tprint(screen,
                         "{:>2} Button RightPad value: {}".format(14, button))
        if button == 1:
            arduino.write("RR-4")
        textPrint.unindent()

        # Hats
        hats = joystick.get_numhats()
        textPrint.tprint(screen, "Number of hats: {}".format(hats))
        textPrint.indent()

        # Hat position. All or nothing for direction, not a float like
        # get_axis(). Position is a tuple of int values (x, y).
        for j in range(hats):
            hat = joystick.get_hat(j)
            textPrint.tprint(screen, "Hat {} value: {}".format(j, str(hat)))
        textPrint.unindent()

        if arduino.in_waiting:
            print(arduino.read())

    #
    # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
    #

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    # Limit to 20 frames per second.
    clock.tick(20)

# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit()
