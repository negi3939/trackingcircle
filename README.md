# get position of circle
 makeするとhoughconvert.outを得ます．  
 $ ./houghconvert.out moviefilename  
 を実行すれば，動画内の1つの円をトラッキングします．フレーム番号，x座標(単位はピクセル)，y座標(単位はピクセル),半径(単位はピクセル)の順にcsvデータとして保存します．  
 $ ./houghconvert.out moviefilename noscreen  
 でGUIを表示せずに変換のみを行います．  