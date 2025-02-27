#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    inited_start_game=0;
    inited_game_ui=0;
    ui->setupUi(this);
    for(int i=1;i<=43;i++){
        QImage image=QImage(QString::asprintf ("://asset/%d.png",i));
        asset[i]=QPixmap::fromImage(image);
    }
    resize(1300,750);
    setWindowTitle("英雄本色");
    int fontId = QFontDatabase::addApplicationFont("://asset/font.ttf");
    QString font = QFontDatabase::applicationFontFamilies(fontId).at(0);
    Cfont.setFamily(font);
    QFile qssfile("://asset/main.qss");
    qssfile.open(QFile::ReadOnly);
    QTextStream qssfiletext(&qssfile);
    QString stylesheet = qssfiletext.readAll();
    this->setStyleSheet(stylesheet);
    qssfile.close();
    grabKeyboard();
    srand(time(NULL));
    connect (&Ctimer, &QTimer::timeout, this, &MainWindow::on_timer);
    Ctimer.setTimerType (Qt::PreciseTimer);
    Ctimer.setSingleShot (true);
    init();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *event){
    QPainter Cpainter(this);
    draw(Cpainter);
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(status==0){
        return;
    }
    if(status==3){
        story_step++;
        update();
        return;
    }
    if(status==4){
        story_step++;
        update();
        return;
    }
    if(status==2){
        init();
        update();
        return;
    }
    int key=event->key();
    switch(key){
    case Qt::Key_Up:
    case Qt::Key_W:
        move(chr,'u');
        update();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        move(chr,'d');
        update();
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        move(chr,'l');
        update();
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        move(chr,'r');
        update();
        break;
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    switch(status){
    case 1:
        Cpoint=event->pos();
        tmp1=(Cpoint.y()-300)/40+1;
        if(selecting_magic){
            if((tmp1>=11)&&(Cpoint.x()>=1200)&&(Cpoint.x()<=1270)){
                selecting_magic=0;
                update();
                break;
            }
            tmp1=(Cpoint.y()-30)/50;
            if(tmp1<=0)break;
            if((Cpoint.x()<=710)&&(Cpoint.x()>=650))tmp2=1;
            else tmp2=2;
            for(int i=1;i<=5;i++){
                if(magics[i].learned)tmp1--;
                if(tmp1==0){
                    if(tmp2==1){
                        result=use_magic(magics[i],chr);
                        if(result)selecting_magic=0;
                    }
                    else if(tmp2==2){
                        result=use_magic10(magics[i],chr);
                        if(result)selecting_magic=0;
                    }
                    break;
                }
            }
            update();
            break;
        }
        if(selecting){
            if(Cpoint.x()<=645)
                tmp2=1;
            else
                tmp2=2;
            selecting=0;
            if(tmp2==1){
                if(using_10)result=use_magic10(*using_magic,chr,select_x,select_y);
                else result=use_magic(*using_magic,chr,select_x,select_y);
            }
            else{
                selecting_magic=1;
            }
            update();
            break;
        }
        if(tmp1<=0)break;
        if(Cpoint.x()<213)
            tmp2=1;
        else if(Cpoint.x()>=213&&Cpoint.x()<283)
            tmp2=2;
        else if(Cpoint.x()>=1198)
            tmp2=3;
        if(tmp1>=11){
            if(Cpoint.x()>=1200){
                if(Cpoint.x()<=1270){
                    selecting_magic=1;
                }
            }
            update();
            return;
        }
        else if((tmp2==1)&&(chr.bag[tmp1].id)){
            if(chr.bag[tmp1].id==6||chr.bag[tmp1].id==7){
                chr.hp+=chr.max_hp*chr.bag[tmp1].add_hp/100;
                if(chr.hp>chr.max_hp)chr.hp=chr.max_hp;
                remove(tmp1,chr);
                update();
                break;
            }
            else if(chr.bag[tmp1].id==12||chr.bag[tmp1].id==13){
                chr.mp+=chr.max_mp*chr.bag[tmp1].add_mp/100;
                if(chr.mp>chr.max_mp)chr.mp=chr.max_mp;
                remove(tmp1,chr);
                update();
                break;
            }
            else if(chr.bag[tmp1].id==5){
                thing=chr.hand;
                chr.hand=chr.bag[tmp1];
                remove(tmp1,chr);
                for(int i=1;i<=10;i++){
                    if(chr.bag[i].id==0){
                        chr.bag[i]=thing;
                        break;
                    }
                }
            }
            else if(chr.bag[tmp1].id==10){
                thing=chr.clothes;
                chr.clothes=chr.bag[tmp1];
                remove(tmp1,chr);
                for(int i=1;i<=10;i++){
                    if(chr.bag[i].id==0){
                        chr.bag[i]=thing;
                        break;
                    }
                }
            }
            else if(chr.bag[tmp1].id==11){
                if(magics[chr.bag[tmp1].magic_id].learned){
                    magics[chr.bag[tmp1].magic_id].xp+=100;
                    level_up(magics[chr.bag[tmp1].magic_id]);
                }
                else{
                    magics[chr.bag[tmp1].magic_id].learned=1;
                }
                remove(tmp1,chr);
            }
            update();
        }
        else if((tmp2==2)&&(chr.bag[tmp1].id)){
            remove(tmp1,chr);
            update();
        }
        else if((tmp1==1)&&(tmp2==3)&&(chr.hand.id)){
            for(int i=1;i<=10;i++){
                if(chr.bag[i].id==0){
                    chr.bag[i]=chr.hand;
                    break;
                }
            }
            chr.hand=hand;
            update();
        }
        else if((tmp1==2)&&(tmp2==3)&&(chr.clothes.id)){
            for(int i=1;i<=10;i++){
                if(chr.bag[i].id==0){
                    chr.bag[i]=chr.clothes;
                    break;
                }
            }
            chr.clothes=hand;
            update();
        }
        break;
    case 2:
        init();
        update();
        break;
    case 3:
        story_step++;
        update();
        break;
    }
}
void MainWindow::init(){
    floor=1;
    testing=0;
    status=-1;
    change_status(0);
    story_step=0;
    init_magic();
    new_map();
    new_setup(chr);
}
void MainWindow::draw(QPainter &Cpainter){
    Cpainter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    Cpainter.setPen(Qt::NoPen);
    Cpainter.drawRect(0,0,1600,1600);
    switch(status){
    case 0://开始
        draw_start_game(Cpainter);
        break;
    case 1://游戏
        draw_map(Cpainter);
        draw_lines(Cpainter);
        draw_ui(Cpainter,chr);
        draw_hero(Cpainter,chr);
        if(selecting)draw_select(Cpainter,select_x,select_y);
        if(fighting)draw_fight(Cpainter);
        if(selecting_magic)draw_select_magic(Cpainter);
        break;
    case 2://游戏结束
        draw_game_over(Cpainter);
        break;
    case 3:
        draw_story(Cpainter);
        break;
    case 4:
        draw_win(Cpainter);
        break;
    }
}
void MainWindow::draw_lines(QPainter &Cpainter){
    Cpen.setStyle(Qt::SolidLine);
    Cpen.setWidth(2);
    Cpen.setBrush(Qt::black);
    Cpen.setCapStyle(Qt::RoundCap);
    Cpen.setJoinStyle(Qt::RoundJoin);
    Cpainter.setPen(Cpen);
    for(int i=10;i<=710;i+=100)
        Cpainter.drawLine(QPoint(300,i),QPoint(1000,i));
    for(int i=300;i<=1000;i+=100)
        Cpainter.drawLine(QPoint(i,10),QPoint(i,710));
}
void MainWindow::draw_map(QPainter &Cpainter){
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            switch(see_map[i][j]){
            case 0://迷雾
                Cpainter.setBrush(QColor::fromRgb(230,230,230));
                Cpainter.drawRect((j-1)*100+300,(i-1)*100+10,100,100);
                break;
            case 1://空地
                break;
            case 2://猪
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[pig[index_map[i][j]].color+7]);
                break;
            case 3://楼梯
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[3]);
                break;
            case 4://宝箱
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[4]);
                break;
            case 5://剑
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[items[index_map[i][j]].material+15]);
                break;
            case 6://小血瓶
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[6]);
                break;
            case 7://大血瓶
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[7]);
                break;
            case 8://boss
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[boss.color+7]);
                break;
            case 9://墙
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[22]);
                break;
            case 10://甲
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[items[index_map[i][j]].material+23]);
                break;
            case 11://魔法书
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[magics[items[index_map[i][j]].magic_id].id+30]);
                break;
            case 12://小蓝瓶
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[36]);
                break;
            case 13://大蓝瓶
                Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[37]);
                break;
            case 14://公主
                if(boss.hp>0)
                    Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[42]);
                else
                    Cpainter.drawPixmap((j-1)*100+301,(i-1)*100+11,98,98,asset[43]);
                break;
            }
        }
    }
}
void MainWindow::draw_hero(QPainter &Cpainter,hero &c){
    Cpainter.drawPixmap((c.y-1)*100+301,(c.x-1)*100+11,98,98,asset[1]);
}
void MainWindow::change_status(int new_status){
    if(status==new_status)return;
    switch(status){
    case 0:destory_start_game();break;
    case 1:destory_game_ui();break;
    }
    status=new_status;
    switch(status){
    case 0:init_start_game();break;
    case 1:init_game_ui();break;
    }
}
void MainWindow::destory_game_ui(){
    game_load_pb.hide();
    game_save_pb.hide();
}
void MainWindow::init_game_ui(){
    if(!inited_game_ui){
        inited_game_ui=1;
        game_load_pb.setParent(this);
        game_load_pb.setText("读档");
        Cfont.setPointSize(20);
        game_load_pb.setFont(Cfont);
        game_load_pb.move(95,716);
        game_load_pb.resize(70,30);
        QObject::connect(&game_load_pb,&QPushButton::clicked,this,&MainWindow::load);
        game_save_pb.setParent(this);
        game_save_pb.setText("存档");
        Cfont.setPointSize(20);
        game_save_pb.setFont(Cfont);
        game_save_pb.move(25,716);
        game_save_pb.resize(70,30);
        QObject::connect(&game_save_pb,&QPushButton::clicked,this,&MainWindow::save);
    }
    game_load_pb.show();
    game_save_pb.show();
}
void MainWindow::draw_ui(QPainter &Cpainter,hero &c){
    Cpainter.setPen(Cpen);//层数
    Cfont.setPointSize(20);
    Cpainter.setFont(Cfont);
    QString floor_text=QString::asprintf("层数：%d",floor);
    Cpainter.drawText(50,55,floor_text);
    Cpainter.setBrush(QColor::fromRgb(255,25,25));//生命值
    Cpainter.setPen(Qt::NoPen);
    if(c.hp>0){
        Cpainter.drawRect(50,110,c.hp*200/c.max_hp,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString hp_text=QString::asprintf("生命：%d",c.hp);
    Cpainter.drawText(50,135,hp_text);
    Cpainter.drawRect(50,110,200,30);
    Cpainter.setBrush(QColor::fromRgb(255,255,25));//攻击力
    Cpainter.setPen(Qt::NoPen);
    if(c.atk>0){
        Cpainter.drawRect(50,200,get_atk(c)*200/c.max_atk,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString atk_text=QString::asprintf("攻击力：%d",get_atk(c));
    Cpainter.drawText(50,225,atk_text);
    Cpainter.drawRect(50,200,200,30);
    Cpainter.setBrush(QColor::fromRgb(25,200,25));//等级
    Cpainter.setPen(Qt::NoPen);
    if(c.xp>0){
        Cpainter.drawRect(1050,30,c.xp*200/c.max_xp,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString xp_text=QString::asprintf("等级：%d",c.level);
    Cpainter.drawText(1050,55,xp_text);
    Cpainter.drawRect(1050,30,200,30);
    Cpainter.setBrush(QColor::fromRgb(0,100,255));//魔法值
    Cpainter.setPen(Qt::NoPen);
    if(c.mp>0){
        Cpainter.drawRect(1050,110,c.mp*200/c.max_mp,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString mp_text=QString::asprintf("魔法值：%d",c.mp);
    Cpainter.drawText(1050,135,mp_text);
    Cpainter.drawRect(1050,110,200,30);
    Cpainter.setBrush(QColor::fromRgb(230,230,230));//防御力
    Cpainter.setPen(Qt::NoPen);
    if(get_def(c)>0){
        Cpainter.drawRect(1050,200,get_def(c)*200/c.max_def,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString def_text=QString::asprintf("防御力：%d",get_def(c));
    Cpainter.drawText(1050,225,def_text);
    Cpainter.drawRect(1050,200,200,30);
    for(int i=1;i<=10;i++){
        if(c.bag[i].id){
            switch(c.bag[i].id){
            case 5:
                switch(c.bag[i].material){
                case 1:
                    Cpainter.drawText(40,i*40+300,"木剑");
                    break;
                case 2:
                    Cpainter.drawText(40,i*40+300,"石剑");
                    break;
                case 3:
                    Cpainter.drawText(40,i*40+300,"铁剑");
                    break;
                case 4:
                    Cpainter.drawText(40,i*40+300,"金剑");
                    break;
                case 5:
                    Cpainter.drawText(40,i*40+300,"钻石剑");
                    break;
                }
                Cpainter.drawText(150,i*40+300,"装备");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            case 6:
                Cpainter.drawText(40,i*40+300,"小血瓶");
                Cpainter.drawText(150,i*40+300,"使用");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            case 7:
                Cpainter.drawText(40,i*40+300,"大血瓶");
                Cpainter.drawText(150,i*40+300,"使用");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            case 10:
                switch(c.bag[i].material){
                case 1:
                    Cpainter.drawText(40,i*40+300,"木甲");
                    break;
                case 2:
                    Cpainter.drawText(40,i*40+300,"皮甲");
                    break;
                case 3:
                    Cpainter.drawText(40,i*40+300,"铁甲");
                    break;
                case 4:
                    Cpainter.drawText(40,i*40+300,"金甲");
                    break;
                case 5:
                    Cpainter.drawText(40,i*40+300,"钻石甲");
                    break;
                }
                Cpainter.drawText(150,i*40+300,"装备");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            case 11:
                switch(c.bag[i].magic_id){
                case 1:
                    Cpainter.drawText(40,i*40+300,"魔光术");
                    break;
                case 2:
                    Cpainter.drawText(40,i*40+300,"闪电术");
                    break;
                case 3:
                    Cpainter.drawText(40,i*40+300,"治疗术");
                    break;
                case 4:
                    Cpainter.drawText(40,i*40+300,"火球术");
                    break;
                case 5:
                    Cpainter.drawText(40,i*40+300,"水流术");
                    break;
                }
                Cpainter.drawText(150,i*40+300,"学习");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            case 12:
                Cpainter.drawText(40,i*40+300,"小蓝瓶");
                Cpainter.drawText(150,i*40+300,"使用");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            case 13:
                Cpainter.drawText(40,i*40+300,"大蓝瓶");
                Cpainter.drawText(150,i*40+300,"使用");
                Cpainter.drawText(220,i*40+300,"丢弃");
                break;
            }
        }
        else break;
    }
    switch(c.hand.id){
    case 0:
        Cpainter.drawText(1050,340,"拳头");
        break;
    case 5:
        switch(c.hand.material){
        case 1:
            Cpainter.drawText(1050,340,"木剑");
            break;
        case 2:
            Cpainter.drawText(1050,340,"石剑");
            break;
        case 3:
            Cpainter.drawText(1050,340,"铁剑");
            break;
        case 4:
            Cpainter.drawText(1050,340,"金剑");
            break;
        case 5:
            Cpainter.drawText(1050,340,"钻石剑");
            break;
        }
        Cpainter.drawText(1205,340,"卸下");
        break;
    }
    switch(c.clothes.id){
    case 0:
        Cpainter.drawText(1050,380,"布衣");
        break;
    case 10:
        switch(c.clothes.material){
        case 1:
            Cpainter.drawText(1050,380,"木甲");
            break;
        case 2:
            Cpainter.drawText(1050,380,"皮甲");
            break;
        case 3:
            Cpainter.drawText(1050,380,"铁甲");
            break;
        case 4:
            Cpainter.drawText(1050,380,"金甲");
            break;
        case 5:
            Cpainter.drawText(1050,380,"钻石甲");
            break;
        }
        Cpainter.drawText(1205,380,"脱下");
        break;
    }
    Cpainter.drawText(1200,740,"法术");
}
void MainWindow::init_story(){
    change_status(3);
    update();
}
void MainWindow::destory_start_game(){
    start_game_pb.hide();
    load_pb.hide();
}
void MainWindow::save_game(QDataStream &w){
    w<<floor;
    w<<chr.x;
    w<<chr.y;
    w<<chr.hp;
    w<<chr.atk;
    w<<chr.max_hp;
    w<<chr.max_atk;
    w<<chr.level;
    w<<chr.max_xp;
    w<<chr.xp;
    w<<chr.max_mp;
    w<<chr.mp;
    w<<chr.max_def;
    w<<chr.def;
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            w<<see_map[i][j];
            w<<real_map[i][j];
            w<<index_map[i][j];
        }
    }
    for(int i=1;i<=9;i++){
        w<<pig[i].x;
        w<<pig[i].y;
        w<<pig[i].hp;
        w<<pig[i].atk;
        w<<pig[i].max_hp;
        w<<pig[i].max_atk;
        w<<pig[i].level;
        w<<pig[i].xp;
        w<<pig[i].color;
    }
    w<<boss.x;
    w<<boss.y;
    w<<boss.hp;
    w<<boss.atk;
    w<<boss.max_hp;
    w<<boss.max_atk;
    w<<boss.level;
    w<<boss.xp;
    w<<boss.color;
    w<<chr.hand.level;
    w<<chr.hand.atk;
    w<<chr.hand.def;
    w<<chr.hand.id;
    w<<chr.hand.add_hp;
    w<<chr.hand.add_mp;
    w<<chr.hand.magic_id;
    w<<chr.hand.material;
    w<<chr.clothes.level;
    w<<chr.clothes.atk;
    w<<chr.clothes.def;
    w<<chr.clothes.id;
    w<<chr.clothes.add_hp;
    w<<chr.clothes.add_mp;
    w<<chr.clothes.magic_id;
    w<<chr.clothes.material;
    for(int i=1;i<=10;i++){
        w<<chr.bag[i].level;
        w<<chr.bag[i].atk;
        w<<chr.bag[i].def;
        w<<chr.bag[i].id;
        w<<chr.bag[i].add_hp;
        w<<chr.bag[i].add_mp;
        w<<chr.bag[i].magic_id;
        w<<chr.bag[i].material;
    }
    for(int i=1;i<=49;i++){
        w<<items[i].level;
        w<<items[i].atk;
        w<<items[i].def;
        w<<items[i].id;
        w<<items[i].add_hp;
        w<<items[i].add_mp;
        w<<items[i].magic_id;
        w<<items[i].material;
    }
    for(int i=1;i<=5;i++){
        w<<magics[i].id;
        w<<magics[i].level;
        w<<magics[i].xp;
        w<<magics[i].max_xp;
        w<<magics[i].cost;
        w<<magics[i].learned;
    }
}
void MainWindow::save(){
    releaseKeyboard();
    auto fileDialog = std::make_unique<QFileDialog> (this);
    fileDialog->setAcceptMode (QFileDialog::AcceptSave);
    fileDialog->setWindowTitle ("选择存档文件");
    fileDialog->setDefaultSuffix("heroes");
    fileDialog->setNameFilter ("英雄本色存档(*.heroes)");
    fileDialog->setFileMode (QFileDialog::AnyFile);
    fileDialog->setViewMode (QFileDialog::Detail);
    if (fileDialog->exec ()) {
        auto st=fileDialog->selectedFiles();
        QString filename=*st.begin();
        QFile file(filename);
        file.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QDataStream w(&file);
        save_game(w);
        file.close();
        update();
    }
    grabKeyboard();
}
void MainWindow::load_game(QDataStream &w){
    w>>floor;
    w>>chr.x;
    w>>chr.y;
    w>>chr.hp;
    w>>chr.atk;
    w>>chr.max_hp;
    w>>chr.max_atk;
    w>>chr.level;
    w>>chr.max_xp;
    w>>chr.xp;
    w>>chr.max_mp;
    w>>chr.mp;
    w>>chr.max_def;
    w>>chr.def;
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            w>>see_map[i][j];
            w>>real_map[i][j];
            w>>index_map[i][j];
        }
    }
    for(int i=1;i<=9;i++){
        w>>pig[i].x;
        w>>pig[i].y;
        w>>pig[i].hp;
        w>>pig[i].atk;
        w>>pig[i].max_hp;
        w>>pig[i].max_atk;
        w>>pig[i].level;
        w>>pig[i].xp;
        w>>pig[i].color;
    }
    w>>boss.x;
    w>>boss.y;
    w>>boss.hp;
    w>>boss.atk;
    w>>boss.max_hp;
    w>>boss.max_atk;
    w>>boss.level;
    w>>boss.xp;
    w>>boss.color;
    w>>chr.hand.level;
    w>>chr.hand.atk;
    w>>chr.hand.def;
    w>>chr.hand.id;
    w>>chr.hand.add_hp;
    w>>chr.hand.add_mp;
    w>>chr.hand.magic_id;
    w>>chr.hand.material;
    w>>chr.clothes.level;
    w>>chr.clothes.atk;
    w>>chr.clothes.def;
    w>>chr.clothes.id;
    w>>chr.clothes.add_hp;
    w>>chr.clothes.add_mp;
    w>>chr.clothes.magic_id;
    w>>chr.clothes.material;
    for(int i=1;i<=10;i++){
        w>>chr.bag[i].level;
        w>>chr.bag[i].atk;
        w>>chr.bag[i].def;
        w>>chr.bag[i].id;
        w>>chr.bag[i].add_hp;
        w>>chr.bag[i].add_mp;
        w>>chr.bag[i].magic_id;
        w>>chr.bag[i].material;
    }
    for(int i=1;i<=49;i++){
        w>>items[i].level;
        w>>items[i].atk;
        w>>items[i].def;
        w>>items[i].id;
        w>>items[i].add_hp;
        w>>items[i].add_mp;
        w>>items[i].magic_id;
        w>>items[i].material;
    }
    for(int i=1;i<=5;i++){
        w>>magics[i].id;
        w>>magics[i].level;
        w>>magics[i].xp;
        w>>magics[i].max_xp;
        w>>magics[i].cost;
        w>>magics[i].learned;
    }
}
void MainWindow::load(){
    auto fileDialog = std::make_unique<QFileDialog> (this);
    fileDialog->setAcceptMode (QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle ("选择读档文件");
    fileDialog->setDefaultSuffix("heroes");
    fileDialog->setNameFilter ("英雄本色存档(*.heroes)");
    fileDialog->setFileMode (QFileDialog::ExistingFile);
    fileDialog->setViewMode (QFileDialog::Detail);
    if (fileDialog->exec ()) {
        auto st=fileDialog->selectedFiles();
        QString filename=*st.begin();
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QDataStream w(&file);
        load_game(w);
        file.close();
        change_status(1);
        update();
    }
}
void MainWindow::init_start_game(){
    if(!inited_start_game){
        inited_start_game=1;
        start_game_pb.setParent(this);
        start_game_pb.setText("开始游戏");
        Cfont.setPointSize(70);
        start_game_pb.setFont(Cfont);
        start_game_pb.move(390,355);
        start_game_pb.resize(500,150);
        QObject::connect(&start_game_pb,&QPushButton::clicked,this,&MainWindow::init_story);
        load_pb.setParent(this);
        load_pb.setText("加载存档");
        Cfont.setPointSize(50);
        load_pb.setFont(Cfont);
        load_pb.move(440,500);
        load_pb.resize(400,100);
        QObject::connect(&load_pb,&QPushButton::clicked,this,&MainWindow::load);
    }
    start_game_pb.show();
    load_pb.show();
}
void MainWindow::draw_start_game(QPainter &Cpainter){
    Cpainter.setPen(Cpen);
    Cfont.setPointSize(150);
    Cpainter.setFont(Cfont);
    Cpainter.drawText(240,300,"英雄本色");
}
void MainWindow::draw_fight(QPainter &Cpainter){
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::white);
    Cpainter.drawRect(350,200,600,300);
    Cpainter.drawPixmap(680,202,200,200,asset[fighting_monster->color+7]);
    Cpainter.setBrush(QColor::fromRgb(255,25,25));//生命值
    Cpainter.setPen(Qt::NoPen);
    if(fighting_monster->hp>0){
        Cpainter.drawRect(400,250,fighting_monster->hp*200/fighting_monster->max_hp,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString hp_text=QString::asprintf("生命：%d",fighting_monster->hp);
    Cpainter.drawText(400,275,hp_text);
    Cpainter.drawRect(400,250,200,30);
    Cpainter.setBrush(QColor::fromRgb(255,255,25));//攻击力
    Cpainter.setPen(Qt::NoPen);
    if(fighting_monster->atk>0){
        Cpainter.drawRect(400,350,fighting_monster->atk*200/fighting_monster->max_atk,30);
    }
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::NoBrush);
    QString atk_text=QString::asprintf("攻击力：%d",fighting_monster->atk);
    Cpainter.drawText(400,375,atk_text);
    Cpainter.drawRect(400,350,200,30);
}
void MainWindow::draw_game_over(QPainter &Cpainter){
    Cpainter.setPen(Cpen);
    Cfont.setPointSize(150);
    Cpainter.setFont(Cfont);
    Cpainter.drawText(240,300,"游戏结束");
}
void MainWindow::draw_story(QPainter &Cpainter){
    Cpainter.setPen(Cpen);
    Cfont.setPointSize(40);
    Cpainter.setFont(Cfont);
    switch(story_step){
    case 7:
        change_status(1);
        update();
        break;
    case 6:Cpainter.drawText(100,500,"于是，英雄开始了他的冒险...");
    case 5:Cpainter.drawText(100,430,"传言，公主被关在地牢的第一百二十层");
    case 4:Cpainter.drawText(100,360,"就把公主许配给他");
    case 3:Cpainter.drawText(100,290,"皇帝许诺，如果有任何人找到了公主");
    case 2:Cpainter.drawText(100,220,"但是，突然有一天，公主被邪恶的魔族抓走了");
    case 1:Cpainter.drawText(100,150,"很久很久以前，一位英雄爱上了美丽的公主");
    }
}
void MainWindow::draw_win(QPainter &Cpainter){
    Cpainter.setPen(Cpen);
    Cfont.setPointSize(40);
    Cpainter.setFont(Cfont);
    switch(story_step){
    case 8:
        init();
        update();
        break;
    case 7:Cpainter.drawText(100,360,"恒瑞游戏 2021");
    case 6:Cpainter.drawText(100,290,"代码：chr");
    case 5:Cpainter.drawText(100,220,"美术：chr");
    case 4:Cpainter.drawText(100,150,"设计：chr");
        break;
    case 3:Cpainter.drawText(100,290,"从此，幸福地生活在森林里");
    case 2:Cpainter.drawText(100,220,"第二天，他们举行了隆重的婚礼");
    case 1:Cpainter.drawText(100,150,"英雄打败了魔族，救出了公主");
    }
}
void MainWindow::new_map(){
    selecting=0;
    selecting_magic=0;
    memset(see_map,0,sizeof(see_map));
    memset(index_map,0,sizeof(index_map));
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++){
            real_map[i][j]=1;
        }
    }
    if(testing){
        floor=100;
        for(int i=1;i<=7;i++){
            real_map[2][i]=4;
            index_map[2][i]=i;
        }
        setup(chr);
        make_pig();
        return;
    }
    if(floor==19){
        real_map[4][7]=3;
        real_map[3][6]=real_map[3][5]=real_map[3][4]=real_map[5][6]=real_map[5][5]=real_map[5][4]=4;
        real_map[3][1]=real_map[3][2]=real_map[3][3]=9;
        real_map[5][1]=real_map[5][2]=real_map[5][3]=9;
        real_map[4][4]=8;
        index_map[3][6]=1;
        index_map[3][5]=2;
        index_map[3][4]=3;
        index_map[5][6]=4;
        index_map[5][5]=5;
        index_map[5][4]=6;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        chr.x=4;
        chr.y=1;
        select_x=chr.x;
        select_y=chr.y;
        boss.max_hp=500;
        boss.max_atk=350;
        boss.atk=40;
        boss.hp=boss.max_hp;
        boss.xp=1000;
        boss.color=14;
        return;
    }
    if(floor==39){
        real_map[4][7]=3;
        real_map[3][6]=real_map[3][5]=real_map[3][4]=real_map[5][6]=real_map[5][5]=real_map[5][4]=4;
        real_map[3][1]=real_map[3][2]=real_map[3][3]=9;
        real_map[5][1]=real_map[5][2]=real_map[5][3]=9;
        index_map[3][6]=1;
        index_map[3][5]=2;
        index_map[3][4]=3;
        index_map[5][6]=4;
        index_map[5][5]=5;
        index_map[5][4]=6;
        real_map[4][4]=8;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        chr.x=4;
        chr.y=1;
        select_x=chr.x;
        select_y=chr.y;
        boss.max_hp=2000;
        boss.max_atk=700;
        boss.atk=170;
        boss.hp=boss.max_hp;
        boss.xp=5000;
        boss.color=22;
        return;
    }
    if(floor==59){
        real_map[4][7]=3;
        real_map[3][6]=real_map[3][5]=real_map[3][4]=real_map[5][6]=real_map[5][5]=real_map[5][4]=4;
        real_map[3][1]=real_map[3][2]=real_map[3][3]=9;
        real_map[5][1]=real_map[5][2]=real_map[5][3]=9;
        index_map[3][6]=1;
        index_map[3][5]=2;
        index_map[3][4]=3;
        index_map[5][6]=4;
        index_map[5][5]=5;
        index_map[5][4]=6;
        real_map[4][4]=8;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        chr.x=4;
        chr.y=1;
        select_x=chr.x;
        select_y=chr.y;
        boss.max_hp=8000;
        boss.max_atk=700;
        boss.atk=170;
        boss.hp=boss.max_hp;
        boss.xp=20000;
        boss.color=31;
        return;
    }
    if(floor==79){
        real_map[4][7]=3;
        real_map[3][6]=real_map[3][5]=real_map[3][4]=real_map[5][6]=real_map[5][5]=real_map[5][4]=4;
        real_map[3][1]=real_map[3][2]=real_map[3][3]=9;
        real_map[5][1]=real_map[5][2]=real_map[5][3]=9;
        index_map[3][6]=1;
        index_map[3][5]=2;
        index_map[3][4]=3;
        index_map[5][6]=4;
        index_map[5][5]=5;
        index_map[5][4]=6;
        real_map[4][4]=8;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        chr.x=4;
        chr.y=1;
        select_x=chr.x;
        select_y=chr.y;
        boss.max_hp=20000;
        boss.max_atk=700;
        boss.atk=200;
        boss.hp=boss.max_hp;
        boss.xp=80000;
        boss.color=32;
        return;
    }
    if(floor==99){
        real_map[4][7]=3;
        real_map[3][6]=real_map[3][5]=real_map[3][4]=real_map[5][6]=real_map[5][5]=real_map[5][4]=4;
        real_map[3][1]=real_map[3][2]=real_map[3][3]=9;
        real_map[5][1]=real_map[5][2]=real_map[5][3]=9;
        index_map[3][6]=1;
        index_map[3][5]=2;
        index_map[3][4]=3;
        index_map[5][6]=4;
        index_map[5][5]=5;
        index_map[5][4]=6;
        real_map[4][4]=8;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        chr.x=4;
        chr.y=1;
        select_x=chr.x;
        select_y=chr.y;
        boss.max_hp=20000;
        boss.max_atk=700;
        boss.atk=300;
        boss.hp=boss.max_hp;
        boss.xp=150000;
        boss.color=33;
        return;
    }
    if(floor==119){
        real_map[4][7]=14;
        real_map[3][6]=real_map[3][5]=real_map[3][4]=real_map[5][6]=real_map[5][5]=real_map[5][4]=4;
        real_map[3][1]=real_map[3][2]=real_map[3][3]=9;
        real_map[5][1]=real_map[5][2]=real_map[5][3]=9;
        index_map[3][6]=1;
        index_map[3][5]=2;
        index_map[3][4]=3;
        index_map[5][6]=4;
        index_map[5][5]=5;
        index_map[5][4]=6;
        real_map[4][4]=8;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        chr.x=4;
        chr.y=1;
        select_x=chr.x;
        select_y=chr.y;
        boss.max_hp=50000;
        boss.max_atk=700;
        boss.atk=400;
        boss.hp=boss.max_hp;
        boss.xp=500000;
        boss.color=34;
        return;
    }
    setup(chr);
    make_pig();
    make_box();
    make_stair();
    see(chr);
}
void MainWindow::see(hero &c){
    for(int i=c.x-1;i<=c.x+1;i++){
        for(int j=c.y-1;j<=c.y+1;j++){
            if((i>=1&&i<=7)&&(j>=1&&j<=7))
                see_map[i][j]=real_map[i][j];
        }
    }
}
int MainWindow::check(int x,int y){
    if(x<1||x>7)return 0;
    if(x<1||y>7)return 0;
    if(real_map[x][y]!=1)return 0;
    if(x==chr.x&&y==chr.y)return 0;
    return 1;
}
void MainWindow::setup(hero &c){
    c.x=1;
    c.y=1;
    select_x=c.x;
    select_y=c.y;
}
void MainWindow::new_setup(hero &c){
    c.max_hp=100;
    c.max_atk=100;
    c.hp=c.max_hp;
    c.atk=5;
    c.level=1;
    c.max_xp=100;
    c.xp=0;
    c.max_mp=30;
    c.mp=10;
    c.max_def=20;
    c.def=0;
    hand.atk=0;
    hand.add_hp=0;
    hand.id=0;
    hand.def=0;
    hand.magic_id=0;
    hand.add_mp=0;
    c.hand=hand;
    c.clothes=hand;
    select_x=c.x;
    select_y=c.y;
    memset(c.bag,0,sizeof(c.bag));
    fighting=0;
    step=1;
}
void MainWindow::move(hero &c, char toward){
    if(status==2)return;
    if(selecting_magic)return;
    if(fighting&&(toward!='f'))return;
    if(toward!='f'){
        next_x=c.x;
        next_y=c.y;
        switch(toward){
        case 'u':
            next_x--;
            break;
        case 'd':
            next_x++;
            break;
        case 'l':
            next_y--;
            break;
        case 'r':
            next_y++;
            break;
        }
    }
    if(selecting){
        if(((next_x>=1)&&(next_x<=7))&&((next_y>=1)&&(next_y<=7))){
            select_x=next_x;
            select_y=next_y;
        }
        return;
    }
    if(((next_x>=1)&&(next_x<=7))&&((next_y>=1)&&(next_y<=7))){
    }else return;
    switch(real_map[next_x][next_y]){
    case 1://空地
        if(check(next_x,next_y)){
            c.x=next_x;
            c.y=next_y;
            see(chr);
        }
        break;
    case 2://猪
        result=fight(chr,pig[index_map[next_x][next_y]]);
        switch(result){
        case 0:break;
        case 1:
            change_status(2);
            update();
            break;
        case 2:
            see_map[next_x][next_y]=real_map[next_x][next_y]=1;
            c.xp+=fighting_monster->xp;
            level_up(c);
            move(c,toward);
            update();
            break;
        }
        break;
    case 3://下楼
        new_map();
        floor++;
        break;
    case 4://宝箱
        thing=open_box(index_map[next_x][next_y]);
        see_map[next_x][next_y]=real_map[next_x][next_y]=thing.id;
        update();
        break;
    case 5://剑
    case 6://小血瓶
    case 7://大血瓶
    case 10://甲
    case 11://魔法书
    case 12://小蓝瓶
    case 13://大蓝瓶
        pick_up(next_x,next_y,chr);
        move(c,toward);
        update();
        break;
    case 8://boss
        result=fight(chr,boss);
        switch(result){
        case 0:break;
        case 1:
            change_status(2);
            update();
            break;
        case 2:
            see_map[next_x][next_y]=real_map[next_x][next_y]=1;
            c.xp+=fighting_monster->xp;
            level_up(c);
            move(c,toward);
            update();
            break;
        }
        break;
    case 9:break;//墙
    case 14:
        change_status(4);
        story_step=0;
        update();
        break;
    }
    select_x=c.x;
    select_y=c.y;
}
void MainWindow::make_pig(){
    for(int i=1;i<=rand()%3+4;i++){
        while(1){
            int x=rand()%7+1;
            int y=rand()%7+1;
            if(check(x,y)){
                int pig_maker=rand()%floor;
                if(pig_maker<=5){
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=100;
                    pig[i].max_hp=30;
                    pig[i].atk=5;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=1;
                    pig[i].xp=20;
                }
                else if(pig_maker<=15){
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=200;
                    pig[i].max_hp=100;
                    pig[i].atk=25;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=2;
                    pig[i].xp=100;
                }
                else if(pig_maker<=25){
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=400;
                    pig[i].max_hp=200;
                    pig[i].atk=40;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=3;
                    pig[i].xp=300;
                }
                else if(pig_maker<=35){
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=800;
                    pig[i].max_hp=500;
                    pig[i].atk=80;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=4;
                    pig[i].xp=800;
                }
                else if(pig_maker<=50){
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=1400;
                    pig[i].max_hp=1000;
                    pig[i].atk=140;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=5;
                    pig[i].xp=1500;
                }
                else if(pig_maker<=65){
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=2000;
                    pig[i].max_hp=2000;
                    pig[i].atk=200;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=6;
                    pig[i].xp=3000;
                }
                else{
                    index_map[x][y]=i;
                    real_map[x][y]=2;
                    pig[i].max_atk=3000;
                    pig[i].max_hp=6000;
                    pig[i].atk=300;
                    pig[i].hp=pig[i].max_hp;
                    pig[i].color=7;
                    pig[i].xp=10000;
                }
                break;
            }
        }
    }
}
void MainWindow::make_stair(){
    while(1){
        int x=rand()%7+1;
        int y=rand()%7+1;
        if(check(x,y)){
            index_map[x][y]=1;
            real_map[x][y]=3;
            break;
        }
    }
}
void MainWindow::make_box(){
    int box_maker=rand()%4;
    for(int i=1;i<=box_maker;i++){
        while(1){
            int x=rand()%7+1;
            int y=rand()%7+1;
            if(check(x,y)){
                index_map[x][y]=i;
                real_map[x][y]=4;
                break;
            }
        }
    }
}
int MainWindow::fight(hero &c, monster &p){
    fighting=1;
    fighting_monster=&p;
    if(step==1){
        p.hp-=get_atk(c);
        if(p.hp<=0){
            step=1;
            fighting=0;
            return 2;
        }
    }else{
        if(p.atk-get_def(c)>=0)
            c.hp-=(p.atk-get_def(c));
        if(c.hp<=0){
            step=1;
            fighting=0;
            return 1;
        }
    }
    update();
    Ctimer.start(100);
    return 0;
}
MainWindow::item MainWindow::open_box(int id){
    item treasure=hand;
    int index=rand()%10;
    int treasure_maker=rand()%floor;
    int magic_maker=rand()%5+1;
    int bottle_maker=rand()%4;
    switch(index){
    case 1:
    case 2://剑
        treasure.id=5;
        if(treasure_maker<=10){
            treasure.atk=5;
            treasure.material=1;
        }
        else if(treasure_maker<=20){
            treasure.atk=20;
            treasure.material=2;
        }
        else if(treasure_maker<=40){
            treasure.atk=50;
            treasure.material=3;
        }
        else{
            treasure.atk=100;
            treasure.material=4;
        }
        break;
    case 3:
    case 4://甲
        treasure.id=10;
        if(treasure_maker<=10){
            treasure.def=2;
            treasure.material=1;
        }
        else if(treasure_maker<=20){
            treasure.def=6;
            treasure.material=2;
        }
        else if(treasure_maker<=40){
            treasure.def=10;
            treasure.material=3;
        }
        else{
            treasure.def=14;
            treasure.material=4;
        }
        break;
    case 5://魔法书
    case 6:
        treasure.id=11;
        treasure.magic_id=magic_maker;
        break;
    case 0:
    case 7://小瓶
        if(bottle_maker<=2){
            treasure.id=6;
            treasure.add_hp=10;
        }
        else{
            treasure.id=12;
            treasure.add_mp=10;
        }
        break;
    case 8:
    case 9://大瓶
        if(bottle_maker<=2){
            treasure.id=7;
            treasure.add_hp=30;
        }
        else{
            treasure.id=13;
            treasure.add_mp=30;
        }
        break;
    }
    items[id]=treasure;
    return treasure;
}
void MainWindow::pick_up(int x, int y, hero &c){
    thing=items[index_map[x][y]];
    for(int i=1;i<=10;i++){
        if(c.bag[i].id==0){
            c.bag[i]=thing;
            break;
        }
    }
    see_map[x][y]=real_map[x][y]=1;
}
int MainWindow::get_atk(hero &c){
    int all_atk=c.atk;
    all_atk+=c.hand.atk;
    if(all_atk>c.max_atk)all_atk=c.max_atk;
    return all_atk;
}
void MainWindow::on_timer(){
    if(step==1)step=2;
    else step=1;
    move(chr,'f');
}
void MainWindow::remove(int id, hero &c){
    for(int i=id;i<10;i++){
        c.bag[i]=c.bag[i+1];
    }
    c.bag[10]=hand;
}
void MainWindow::level_up(hero &c){
    while(c.xp>=c.max_xp){
        c.xp-=c.max_xp;
        c.level++;
        c.atk+=5;
        c.max_atk+=10;
        c.hp+=50;
        c.max_hp+=50;
        c.max_xp+=100;
        c.max_mp+=10;
        c.max_def+=2;
        c.def+=1;
        c.mp+=5;
    }
}
void MainWindow::level_up(magic &m){
    while(m.xp>=m.max_xp){
        m.xp-=m.max_xp;
        m.level++;
        m.max_xp+=100;
        switch(m.id){
        case 1:
            m.cost-=1;
            if(m.cost<0)m.cost=0;
            break;
        }
    }
}
int MainWindow::get_def(hero &c){
    int all_def=c.def;
    all_def+=c.clothes.def;
    if(all_def>c.max_def)all_def=c.max_def;
    return all_def;
}
void MainWindow::draw_select_magic(QPainter &Cpainter){
    int magic_null=1;
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::white);
    Cpainter.drawRect(100,50,1100,660);
    int tmpy=130;
    for(int i=1;i<=5;i++){
        if(magics[i].learned){
            magic_null=0;
            switch(i){
            case 1:
                Cpainter.drawText(140,tmpy,"魔光术");
                break;
            case 2:
                Cpainter.drawText(140,tmpy,"闪电术");
                break;
            case 3:
                Cpainter.drawText(140,tmpy,"治疗术");
                break;
            case 4:
                Cpainter.drawText(140,tmpy,"火球术");
                break;
            case 5:
                Cpainter.drawText(140,tmpy,"水流术");
                break;
            }
            Cpainter.setBrush(QColor::fromRgb(25,200,25));//等级
            Cpainter.setPen(Qt::NoPen);
            if(magics[i].xp>0){
                Cpainter.drawRect(240,tmpy-25,magics[i].xp*200/magics[i].max_xp,30);
            }
            Cpainter.setPen(Cpen);
            Cpainter.setBrush(Qt::NoBrush);
            QString xp_text=QString::asprintf("等级：%d",magics[i].level);
            Cpainter.drawText(240,tmpy,xp_text);
            Cpainter.drawRect(240,tmpy-25,200,30);
            QString cost_text=QString::asprintf("消耗：%d",magics[i].cost);
            Cpainter.drawText(450,tmpy,cost_text);
            Cpainter.drawText(650,tmpy,"使用");
            Cpainter.drawText(720,tmpy,"使用 X 10");
            tmpy+=50;
        }
    }
    if(magic_null)Cpainter.drawText(140,tmpy,"暂无法术");
}
void MainWindow::init_magic(){
    for(int i=1;i<=5;i++){
        magics[i].learned=0;
        magics[i].level=1;
        magics[i].xp=0;
        magics[i].max_xp=100;
        magics[i].cost=10;
        magics[i].id=i;
    }
}
int MainWindow::use_magic(magic &m,hero &c){
    using_10=0;
    if(c.mp<m.cost)
        return 0;
    switch(m.id){
    case 1://魔光术
        c.mp-=m.cost;
        for(int i=1;i<=7;i++){
            for(int j=1;j<=7;j++){
                see_map[i][j]=real_map[i][j];
            }
        }
        m.xp+=20;
        level_up(m);
        break;
    case 3://治疗术
        c.mp-=m.cost;
        c.hp+=c.max_hp*(5+5*m.level)/100;
        if(c.hp>c.max_hp)c.hp=c.max_hp;
        m.xp+=20;
        level_up(m);
        break;
    case 2:
    case 4:
    case 5:
        using_magic=&m;
        selecting=1;
        break;
    }
    return 1;
}
int MainWindow::use_magic10(magic &m,hero &c){
    using_10=1;
    if(c.mp<m.cost*10)
        return 0;
    switch(m.id){
    case 1://魔光术
        c.mp-=m.cost*10;
        for(int k=1;k<=10;k++){
            for(int i=1;i<=7;i++){
                for(int j=1;j<=7;j++){
                    see_map[i][j]=real_map[i][j];
                }
            }
        }
        m.xp+=200;
        level_up(m);
        break;
    case 3://治疗术
        c.mp-=m.cost*10;
        c.hp+=c.max_hp*(5+5*m.level)/100*10;
        if(c.hp>c.max_hp)c.hp=c.max_hp;
        m.xp+=200;
        level_up(m);
        break;
    case 2:
    case 4:
    case 5:
        using_magic=&m;
        selecting=1;
        break;
    }
    return 1;
}
int MainWindow::use_magic10(magic &m, hero &c, int x, int y){
    if(c.mp<m.cost*10)return 0;
    for(int i=1;i<=10;i++){
        use_magic(m,c,x,y);
    }
    return 1;
}
int MainWindow::use_magic(magic &m, hero &c, int x, int y){
    if(c.mp<m.cost)
        return 0;
    switch(m.id){
    case 2://闪电术
        c.mp-=m.cost;
        damage(c,x,y,25+25*m.level);
        damage(c,x+1,y,25+25*m.level);
        damage(c,x,y+1,25+25*m.level);
        damage(c,x-1,y,25+25*m.level);
        damage(c,x,y-1,25+25*m.level);
        damage(c,x+1,y+1,25+25*m.level);
        damage(c,x-1,y+1,25+25*m.level);
        damage(c,x-1,y-1,25+25*m.level);
        damage(c,x+1,y-1,25+25*m.level);
        m.xp+=20;
        level_up(m);
        break;
    case 4://火球术
        c.mp-=m.cost;
        damage(c,x,y,50+50*m.level);
        m.xp+=20;
        level_up(m);
        break;
    case 5://水流术
        c.mp-=m.cost;
        if((c.x==x)&&(c.y==y))
            damage(c,x,y,25+25*m.level);
        else if(c.x==x-1){
            while(x<=7){
                damage(c,x,y,25+25*m.level);
                x++;
            }
        }
        else if(c.x==x+1){
            while(x>=1){
                damage(c,x,y,25+25*m.level);
                x--;
            }
        }
        else if(c.y==y-1){
            while(y<=7){
                damage(c,x,y,25+25*m.level);
                y++;
            }
        }
        else if(c.y==y+1){
            while(y>=1){
                damage(c,x,y,25+25*m.level);
                y--;
            }
        }
        m.xp+=20;
        level_up(m);
        break;
    }
    return 1;
}
void MainWindow::draw_select(QPainter &Cpainter,int x,int y){
    Cpainter.setPen(Cpen);
    Cpainter.setBrush(Qt::white);
    Cpainter.drawRect(560,40,180,30);
    Cpainter.drawText(570,65,"施法");
    Cpainter.drawText(670,65,"取消");
    Cpainter.drawLine((y-1)*100+311,(x-1)*100+21,(y-1)*100+321,(x-1)*100+21);
    Cpainter.drawLine((y-1)*100+311,(x-1)*100+21,(y-1)*100+311,(x-1)*100+31);
    Cpainter.drawLine((y-1)*100+381,(x-1)*100+21,(y-1)*100+391,(x-1)*100+21);
    Cpainter.drawLine((y-1)*100+391,(x-1)*100+21,(y-1)*100+391,(x-1)*100+31);
    Cpainter.drawLine((y-1)*100+311,(x-1)*100+101,(y-1)*100+321,(x-1)*100+101);
    Cpainter.drawLine((y-1)*100+311,(x-1)*100+91,(y-1)*100+311,(x-1)*100+101);
    Cpainter.drawLine((y-1)*100+381,(x-1)*100+101,(y-1)*100+391,(x-1)*100+101);
    Cpainter.drawLine((y-1)*100+391,(x-1)*100+91,(y-1)*100+391,(x-1)*100+101);
}
void MainWindow::damage(hero &c, int x, int y,int atk){
    if(real_map[x][y]!=2&&real_map[x][y]!=8)return;
    if(real_map[x][y]==2)
        fighting_monster=&pig[index_map[x][y]];
    else if(real_map[x][y]==8){
        fighting_monster=&boss;
    }
    fighting_monster->hp-=atk;
    if(fighting_monster->hp<=0){
        see_map[x][y]=real_map[x][y]=1;
        c.xp+=fighting_monster->xp;
        level_up(c);
    }
}
