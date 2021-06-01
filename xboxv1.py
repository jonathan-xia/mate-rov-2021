import pygame
import serial

# Define some colors.
BLACK = pygame.Color('black')
WHITE = pygame.Color('white')

Axis1 = 0
Axis0 = 0
Axis4 = 0
ser = serial.Serial('/dev/tty.usbmodem141201')  # open serial port()


def message(msg):
    ser.write(str(msg).encode())  # send the serial message
    # ser.close()
    print(msg)


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
print("ready")
print("wait for arduino")
while True:
    if ser.in_waiting:
        break
print("arduino ready")
stopped = [True] * 10
# -------- Main Program Loop -----------
while not done:
    # Possible joystick actions: JOYAXISMOTION, JOYBALLMOTION, JOYBUTTONDOWN,
    # JOYBUTTONUP, JOYHATMOTION
    for event in pygame.event.get():  # User did something.
        # print("event happened")
        # print(event.type)
        if event.type == pygame.QUIT:  # If user clicked close.
            done = True  # Flag that we are done so we exit this loop.

        # elif event.type == pygame.JO YBUTTONDOWN:
        #     print("Joystick button pressed.")
        # elif event.type == pygame.JOYBUTTONUP:
        #     print("Joystick button released.")

        screen.fill(WHITE)
        textPrint.reset()
        # print(pygame.joystick)
        # Get count of joysticks.
        joystick_count = pygame.joystick.get_count()
        # print(joystick_count)
        textPrint.tprint(screen, "Number of joysticks: {}".format(joystick_count))
        textPrint.indent()

        # For each joystick:
        for i in range(joystick_count):
            # print("joystick")
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

            for i in range(axes):
                while ser.in_waiting:
                    print(ser.readline())
                axis = joystick.get_axis(i)
                textPrint.tprint(screen, "Axis {} value: {:>6.3f}".format(i, axis))
                if i == 2 or i == 5:
                    if axis < -0.9:
                        if not stopped[i]:
                            stopped[i] = True
                            message(str(i) + " 0" + "x")
                        continue
                    stopped[i] = False
                    message(str(i) + " " + str(axis) + "x")
                else:
                    if axis < 0.1 and axis > -0.1:
                        if not stopped[i]:
                            stopped[i] = True
                            message(str(i) + " 0" + "x")
                        continue
                    stopped[i] = False
                    message(str(i) + " " + str(axis) + "x")
            textPrint.unindent()


            #buttons = joystick.get_numbuttons()
            #textPrint.tprint(screen, "Number of buttons: {}".format(buttons))
            #textPrint.indent()

            #for i in range(buttons):
            #  button = joystick.get_button(i)
            #  textPrint.tprint(screen,"Button {:>2} value: {}".format(i, button))

            #if (i, buttons) == (3, 1):
            #    print("Claw Open")  # 66
            #    message('B')
            #elif (i, buttons) == (1, 1):
            #    print("Claw Close")  # 86
            #    message('V')
            #   if(i, button) == (5, 1): #72 H
            #     print("Roll Right")
            #     message('H')# HACK:

            #   elif(i, button) == (4, 1):
            #     print("Roll Left") #71 G
            #     message('G')
            #   elif(i, button) == (3, 1):
            #     print("Claw Open") #66
            #     message('B')
            #   elif(i, button) == (1, 1):
            #     print("Claw Close") #86
            #     message('V')
            # textPrint.unindent()
            #
            # hats = joystick.get_numhats()
            # textPrint.tprint(screen, "Number of hats: {}".format(hats))
            # textPrint.indent()
            #
            # # Hat position. All or nothing for direction, not a float like
            # # get_axis(). Position is a tuple of int values (x, y).
            # for i in range(hats):
            #   hat = joystick.get_hat(i)
            #   textPrint.tprint(screen, "Hat {} value: {}".format(i, str(hat)))
            #   if (hat) == (0, 1):
            #       print("Heave Up")
            #       message('I') #73
            #   if (hat) == (0, -1):
            #       print("Heave Down")
            #       message('K')#75
            # textPrint.unindent()
            #
            # textPrint.unindent()

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
