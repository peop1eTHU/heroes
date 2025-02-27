#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QFontDatabase>
#include <QTimer>
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>

#include <random>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    struct magic{
        int id;
        int level;
        int xp;
        int max_xp;
        int cost;
        int learned;
    }magics[6];
    struct item{
        int level;
        int atk;
        int def;
        int id;
        int add_hp;
        int add_mp;
        int magic_id;
        int material;
    }items[50],hand;
    struct hero{
        int x;
        int y;
        int hp;
        int atk;
        int max_hp;
        int max_atk;
        int level;
        int max_xp;
        int xp;
        int max_mp;
        int mp;
        int max_def;
        int def;
        item hand;
        item clothes;
        item bag[11];
    }chr;
    struct monster{
        int x;
        int y;
        int hp;
        int atk;
        int max_hp;
        int max_atk;
        int level;
        int xp;
        int color;
    }pig[10];
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void draw(QPainter &Cpainter);
    void draw_lines(QPainter &Cpainter);
    void draw_map(QPainter &Cpainter);
    void draw_hero(QPainter &Cpainter,hero &c);
    void draw_ui(QPainter &Cpainter,hero &c);
    void init_start_game();
    void draw_start_game(QPainter &Cpainter);
    void draw_game_over(QPainter &Cpainter);
    void draw_fight(QPainter &Cpainter);
    void draw_story(QPainter &Cpainter);
    void draw_win(QPainter &Cpainter);
    void draw_select_magic(QPainter &Cpainter);
    void destory_start_game();
    void init_story();
    void draw_select(QPainter &Cpainter,int x,int y);
    void new_map();
    void init();
    void init_game_ui();
    void destory_game_ui();
    void see(hero &c);
    void new_setup(hero &c);
    void setup(hero &c);
    void move(hero &c,char toward);
    int check(int x,int y);
    void make_pig();
    void make_stair();
    void make_box();
    void on_timer();
    void pick_up(int x,int y,hero &c);
    void remove(int id,hero &c);
    void level_up(hero &c);
    void level_up(magic &m);
    void init_magic();
    int use_magic(magic &m,hero &c);
    int use_magic(magic &m,hero &c,int x,int y);
    int use_magic10(magic &m,hero &c);
    int use_magic10(magic &m,hero &c,int x,int y);
    void damage(hero &c,int x,int y,int atk);
    void load();
    void load_game(QDataStream &w);
    void save();
    void save_game(QDataStream &w);
    void change_status(int new_status);
    item open_box(int id);
    int fight(hero &c,monster &p);
    int get_atk(hero &c);
    int get_def(hero &c);
    int testing;
    int see_map[10][10];
    int real_map[10][10];
    int index_map[10][10];
    int floor;
    int result;
    int status;
    int fighting;
    int selecting;
    int select_x;
    int select_y;
    int selecting_magic;
    int step;
    int story_step;
    int next_x;
    int next_y;
    int tmp1;
    int tmp2;
    bool using_10;
    bool inited_game_ui;
    bool inited_start_game;
    item thing;
    monster boss;
    monster* fighting_monster;
    magic* using_magic;
    QPixmap asset[50];
    QFont Cfont;
    QPen Cpen;
    QTimer Ctimer;
    QPoint Cpoint;
    QPushButton start_game_pb;
    QPushButton load_pb;
    QPushButton game_load_pb;
    QPushButton game_save_pb;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
