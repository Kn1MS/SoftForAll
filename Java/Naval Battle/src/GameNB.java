import jdk.nashorn.internal.ir.Block;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.lang.reflect.Array;
import java.util.*;

/**
 * Created by Vasar on 24.02.2017.
 */
public class GameNB {
    private final String TITLE_OF_PROGRAM = "Naval Battle";
    private final int BLOCK_SIZE = 50;
    private final int ARC_RADIUS = 6;

    private final int FIELD_WIDTH = 23; // X Размеры поля
    private final int FIELD_HEIGHT = 11; // Y

    private final int FIELD_DX = 4;
    private final int FIELD_DY = 27;

    private final int LEFT = 37; // Коды клавиш.
    private final int UP = 38;
    private final int RIGHT = 39;
    private final int DOWN = 40;

    private final int SHOW_DELAY = 100; // Задержка анимации.

    private final int[] SCORES = {100, 300, 700, 1500}; // Массив очков.
    private int gameScore = 0; // Текущие очки игрока.
    private int[][] battlefield = new int[FIELD_HEIGHT - 1][FIELD_WIDTH / 2 - 1]; // Само поле для моркого боя 1.
    private int[][] battlefield2 = new int[FIELD_HEIGHT - 1][FIELD_WIDTH / 2 - 1]; // Само поле для моркого боя 2.
    private String[] characters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    private String[] numbers = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    private ArrayList<Ship> ships = new ArrayList<>();

    private JFrame frame;
    private Canvas canvas = new Canvas();
    private Random random = new Random();

    private Ship ship1 = new Ship(4, false, 1);
    private Ship ship2 = new Ship(3, true, 2);
    private Ship ship3 = new Ship(3, true, 3);
    private Ship ship4 = new Ship(2, true, 4);
    private Ship ship5 = new Ship(2, false, 5);
    private Ship ship6 = new Ship(2, false, 6);
    private Ship ship7 = new Ship(1, true, 7);
    private Ship ship8 = new Ship(1, true, 8);
    private Ship ship9 = new Ship(1, true, 9);
    private Ship ship10 = new Ship(1, true, 10);

    private Ship ship11 = new Ship(4, false, 11);
    private Ship ship12 = new Ship(3, true, 12);
    private Ship ship13 = new Ship(3, true, 13);
    private Ship ship14 = new Ship(2, true, 14);
    private Ship ship15 = new Ship(2, false, 15);
    private Ship ship16 = new Ship(2, false, 16);
    private Ship ship17 = new Ship(1, true, 17);
    private Ship ship18 = new Ship(1, true, 18);
    private Ship ship19 = new Ship(1, true, 19);
    private Ship ship20 = new Ship(1, true, 20);

    private boolean gameOver = false;
    private boolean step = false;
    private final int[][] GAME_OVER_MSG = {
            {0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,0,1,1,0},
            {1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1},
            {1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1},
            {1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,0},
            {0,1,1,0,0,1,0,0,1,0,1,0,1,0,1,0,0,1,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,1,1,0,0,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0},
            {1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0},
            {1,0,0,1,0,1,0,1,0,0,1,1,1,1,0,1,1,1,0,0},
            {1,0,0,1,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0},
            {0,1,1,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0}}; // Вывод надписи "конец игры".

    public static void main(String[] args) {
        new GameNB().go();
    }

    private void go() {
        ships.add(ship1);
        ships.add(ship2);
        ships.add(ship3);
        ships.add(ship4);
        ships.add(ship5);
        ships.add(ship6);
        ships.add(ship7);
        ships.add(ship8);
        ships.add(ship9);
        ships.add(ship10);
        frame = new JFrame(TITLE_OF_PROGRAM);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setSize(FIELD_WIDTH * BLOCK_SIZE + FIELD_DX, FIELD_HEIGHT * BLOCK_SIZE + FIELD_DY);
        frame.setResizable(false);
        canvas.setBackground(Color.decode("#000080"));
        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (!gameOver) {
                    if (e.getKeyCode() == DOWN) {

                    }
                    if (e.getKeyCode() == UP) {

                    }
                    if ((e.getKeyCode() == LEFT) || (e.getKeyCode() == RIGHT)) {

                    }
                }
            }
        });
        frame.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (step) {
                    int dx = ((e.getX() - FIELD_DX) / BLOCK_SIZE) - 12, dy = ((e.getY() - FIELD_DY) / BLOCK_SIZE) - 1;
                    if (((dx >= 0) && (dx < FIELD_WIDTH / 2 - 1)) && ((dy >= 0) && (dy < FIELD_HEIGHT - 1))) {
                        checkField(battlefield2, dx, dy, step);
                    }
                }
            }
        });
        frame.getContentPane().add(BorderLayout.CENTER, canvas);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        // Главный цикл игры.
        while (!gameOver) {
            try {
                Thread.sleep(SHOW_DELAY);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
            canvas.repaint();
            if (!step) {
                int dx = random.nextInt(10);
                int dy = random.nextInt(10);
                while ((battlefield[dy][dx] == 2) || (battlefield[dy][dx] == 3)) {
                    dx = random.nextInt(10);
                    dy = random.nextInt(10);
                }
                checkField(battlefield, dx, dy, step);
            }
        }
    }

    class Ship {
        private ArrayList<Block> figure = new ArrayList<>();
        private int color;
        private int x = 0, y = 0;

        Ship(int sizeOfShip, boolean vertical, int number) {
            color = 0x8D948D; // 0xFFFFFF;
            switch (number) {
                case 1:
                    x = 6; y = 1;
                    break;
                case 2:
                    x = 0; y = 3;
                    break;
                case 3:
                    x = 8; y = 4;
                    break;
                case 4:
                    x = 1; y = 8;
                    break;
                case 5:
                    x = 2; y = 6;
                    break;
                case 6:
                    x = 8; y = 9;
                    break;
                case 7:
                    x = 1; y = 1;
                    break;
                case 8:
                    x = 4; y = 0;
                    break;
                case 9:
                    x = 6; y = 4;
                    break;
                case 10:
                    x = 4; y = 8;
                    break;
                case 11:
                    x = 6; y = 1;
                    break;
                case 12:
                    x = 0; y = 3;
                    break;
                case 13:
                    x = 8; y = 4;
                    break;
                case 14:
                    x = 1; y = 8;
                    break;
                case 15:
                    x = 2; y = 6;
                    break;
                case 16:
                    x = 8; y = 9;
                    break;
                case 17:
                    x = 1; y = 1;
                    break;
                case 18:
                    x = 4; y = 0;
                    break;
                case 19:
                    x = 6; y = 4;
                    break;
                case 20:
                    x = 4; y = 8;
                    break;
            }
            if (vertical) {
                for (int i = 0; i < sizeOfShip; i++) {
                    figure.add(new Block(x, y + i));
                    if (number <= 10)
                        battlefield[y + i][x] = 1;
                    else
                        battlefield2[y + i][x] = 1;
                }
            }
            else {
                for (int i = 0; i < sizeOfShip; i++) {
                    figure.add(new Block(x + i, y));
                    if (number <= 10)
                        battlefield[y][x + i] = 1;
                    else
                        battlefield2[y][x + i] = 1;
                }
            }
        }

        boolean isDead(int[][] batfd) {
            for (Block block : figure)
                if (batfd[block.getY()][block.getX()] == 1)
                    return false;
            return true;
        }

        void paint(Graphics g) {
            for (Block block : figure)
                block.pain(g, color);
        }
    }

    class Block {
        private int x, y;

        Block(int x, int y) {
            setX(x);
            setY(y);
        }

        void setX(int x) {
            this.x = x;
        }
        void setY(int y) {
            this.y = y;
        }

        int getX() {
            return x;
        }
        int getY() {
            return y;
        }

        void pain(Graphics g, int color) {
            g.setColor(new Color(color));
            //g.drawRoundRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, ARC_RADIUS, ARC_RADIUS);
            g.fill3DRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1, true);
        }
    }

    private Ship getShip(int x, int y) {
        for (Ship ship : ships)
            for (Block block : ship.figure)
                if ((block.getX() == x) && (block.getY() == y))
                    return ship;
        return null;
    }

    private void checkField(int[][] batfd, int dx, int dy , boolean nextStep) { // Проверка на попадание по кораблю.
        int countDeath = 0, countDeathNew = 0;
        for (Ship ship : ships)
            if (ship.isDead(batfd))
                countDeath++;
        if ((batfd[dy][dx] != 1) && (batfd[dy][dx] != 2) && (batfd[dy][dx] != 3))
            step = !nextStep;
        if (batfd[dy][dx] == 0)
            batfd[dy][dx] = 3;
        if (batfd[dy][dx] == 1) {
            batfd[dy][dx] = 2;
            for (Ship ship : ships)
                if (ship.isDead(batfd))
                    countDeathNew++;
            if (countDeathNew > countDeath) {
                Ship ship = getShip(dx, dy);
                if (ship != null)
                    for (Block block : ship.figure) {
                        int dy1 = block.getY() - 1, dy2 = block.getY() + 1;
                        int dx1 = block.getX() - 1, dx2 = block.getX() + 1;
                        if (dy1 >= 0)
                            if ((batfd[dy1][block.getX()] != 1) && (batfd[dy1][block.getX()] != 2))
                                batfd[dy1][block.getX()] = 3;
                        if (dx1 >= 0)
                            if ((batfd[block.getY()][dx1] != 1) && (batfd[block.getY()][dx1] != 2))
                                batfd[block.getY()][dx1] = 3;
                        if ((dy1 >= 0) && (dx1 >= 0))
                            if ((batfd[dy1][dx1] != 1) && (batfd[dy1][dx1] != 2))
                                batfd[dy1][dx1] = 3;
                        if (dy2 <= 9)
                            if ((batfd[dy2][block.getX()] != 1) && (batfd[dy2][block.getX()] != 2))
                                batfd[dy2][block.getX()] = 3;
                        if (dx2 <= 9)
                            if ((batfd[block.getY()][dx2] != 1) && (batfd[block.getY()][dx2] != 2))
                                batfd[block.getY()][dx2] = 3;
                        if ((dy2 <= 9) && (dx2 <= 9))
                            if ((batfd[dy2][dx2] != 1) && (batfd[dy2][dx2] != 2))
                                batfd[dy2][dx2] = 3;
                        if ((dy1 >= 0) && (dx2 <= 9))
                            if ((batfd[dy1][dx2] != 1) && (batfd[dy1][dx2] != 2))
                                batfd[dy1][dx2] = 3;
                        if ((dy2 <= 9) && (dx1 >= 0))
                            if ((batfd[dy2][dx1] != 1) && (batfd[dy2][dx1] != 2))
                                batfd[dy2][dx1] = 3;
                    }
                if (countDeathNew == 10)
                    gameOver = true;
            }
            else {
                int dy1 = dy - 1, dy2 = dy + 1;
                int dx1 = dx - 1, dx2 = dx + 1;
                if ((dy1 >= 0) && (dx1 >= 0))
                    if ((batfd[dy1][dx1] != 1) && (batfd[dy1][dx1] != 2))
                        batfd[dy1][dx1] = 3;
                if ((dy2 <= 9) && (dx2 <= 9))
                    if ((batfd[dy2][dx2] != 1) && (batfd[dy2][dx2] != 2))
                        batfd[dy2][dx2] = 3;
                if ((dy1 >= 0) && (dx2 <= 9))
                    if ((batfd[dy1][dx2] != 1) && (batfd[dy1][dx2] != 2))
                        batfd[dy1][dx2] = 3;
                if ((dy2 <= 9) && (dx1 >= 0))
                    if ((batfd[dy2][dx1] != 1) && (batfd[dy2][dx1] != 2))
                        batfd[dy2][dx1] = 3;
            }
        }
    }

    public class Canvas extends JPanel {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            g.setColor(Color.lightGray);
            g.drawLine(BLOCK_SIZE , BLOCK_SIZE, FIELD_WIDTH * BLOCK_SIZE, BLOCK_SIZE);
            g.drawLine(BLOCK_SIZE , BLOCK_SIZE, BLOCK_SIZE, FIELD_HEIGHT * BLOCK_SIZE);
            g.drawLine((FIELD_WIDTH - 1) * BLOCK_SIZE , BLOCK_SIZE, (FIELD_WIDTH - 1) * BLOCK_SIZE, FIELD_HEIGHT * BLOCK_SIZE);
            Font font = g.getFont();
            g.setFont(new Font("TimesRoman", Font.PLAIN, 25));
            for (int x = 1; x < FIELD_WIDTH / 2; x++) {
                int i = x - 1;
                g.drawString(characters[i], x * BLOCK_SIZE + 20, BLOCK_SIZE - 15);
                g.drawString(characters[i], (x + 11) * BLOCK_SIZE + 20, BLOCK_SIZE - 15);
            }
            for (int y = 1; y < FIELD_HEIGHT; y++) {
                int i = y - 1;
                if (y == FIELD_HEIGHT - 1) {
                    g.drawString(numbers[i], BLOCK_SIZE - 40, y * BLOCK_SIZE + 35);
                    g.drawString(numbers[i], FIELD_WIDTH * BLOCK_SIZE - 40, y * BLOCK_SIZE + 35);
                }
                else {
                    g.drawString(numbers[i], BLOCK_SIZE - 33, y * BLOCK_SIZE + 35);
                    g.drawString(numbers[i], FIELD_WIDTH * BLOCK_SIZE - 33, y * BLOCK_SIZE + 35);
                }
            }
            g.setFont(font);
            for (int x = 0; x < FIELD_WIDTH; x++)
                for (int y = 0; y < FIELD_HEIGHT; y++) {
                    if ((x < FIELD_WIDTH - 1) && (y < FIELD_HEIGHT - 1)) {
                        g.setColor(Color.lightGray);
                        g.drawLine((x + 1) * BLOCK_SIZE - 2, (y + 1) * BLOCK_SIZE, (x + 1) * BLOCK_SIZE + 2, (y + 1) * BLOCK_SIZE);
                        g.drawLine((x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE - 2, (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE + 2);
                    }
                    if (((x == 0) && (y == 0)) || ((x == FIELD_WIDTH - 1) && (y == 0)) || (x == FIELD_WIDTH /2)) {
                        g.setColor(Color.white);
                        g.fill3DRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1, true);
                    }
                    if ((x < FIELD_WIDTH / 2 - 1) && (y < FIELD_HEIGHT - 1)) {
                        if (battlefield[y][x] > 0) {
                            if (battlefield[y][x] == 1)
                                g.setColor(new Color(0x8D948D));
                            else if (battlefield[y][x] == 2)
                                g.setColor(new Color(0xFF7C38));
                            else if (battlefield[y][x] == 3)
                                g.setColor(new Color(0x23AEFF));
                            g.fill3DRect((x + 1) * BLOCK_SIZE + 1, (y + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1, true);
                        }
                        if (battlefield2[y][x] > 0) {
                            if (battlefield2[y][x] == 1)
                                g.setColor(new Color(0x8D948D));
                            else if (battlefield2[y][x] == 2) {
                                g.setColor(new Color(0xFF7C38));
                                g.fill3DRect((x + 12) * BLOCK_SIZE + 1, (y + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1, true);
                            }
                            else if (battlefield2[y][x] == 3) {
                                g.setColor(new Color(0x23AEFF));
                                g.fill3DRect((x + 12) * BLOCK_SIZE + 1, (y + 1) * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1, true);
                            }
                        }
                    }
                }
            if (gameOver) {
                g.setColor(Color.red);
                for (int y = 0; y < GAME_OVER_MSG.length; y++)
                    for (int x = 0; x < GAME_OVER_MSG[y].length; x++)
                        if (GAME_OVER_MSG[y][x] == 1)
                            g.fill3DRect(x * 11 + 475, y * 11 + 200, 12, 12, true);
            }
            else {
                //ship.paint(g);
            }
        }
    }
}