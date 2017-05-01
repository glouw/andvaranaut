import pygame

pygame.init()
screen = pygame.display.set_mode((400, 300))
done = False

while not done:
    # get mouse input
    screen.fill((0,0,0))
    mpos = pygame.mouse.get_pos()
    print mpos
    # Draw a square
    x = mpos[0]
    y = mpos[1]
    pygame.draw.rect(screen, (0, 128, 255), pygame.Rect(x+0,y+0,x+30,y+30))
    # Get keyboard input
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        
        # Update the screen
        pygame.display.flip()
