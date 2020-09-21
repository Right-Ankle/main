指定された符号化レートの品質Nに基づき，入力画像をICAとDCTの基底を併用して符号化します．
その際，得られた64個のICA基底の中から重要なICA基底を探索します

[注意点]
ICA基底を導出する際のパラメータである学習係数などは画像によって適切な値が異なっていますが，
先行研究で用いられた方法に倣い，このプログラムでは様々な画像を使用することを想定し，パラメータを固定しています．

[使用準備]
すべてのCファイル，ヘッダファイルを一つのプロジェクトに追加してください．
使用したい画像（256*256，8階調のみ）を同じフォルダ内に入れておいてください．

[make file name]
*gray
*image
*zansa_sai
*sai
*zansa
*segmentation_pr
*segmentation_co
*final_sai
etc...

[実行結果]
プログラムを実行すると以下のファイルが生成されます．

■ICA_BASIS.pgm　＿＿＿　ICA基底の画像
■ICA_SAI.pgm　＿＿＿　ICA基底とICA係数の線形和による再構成画像
■PCA_BASIS.pgm　＿＿＿　PCA基底の画像
■y.csv　＿＿＿　ICA係数
■Matching_Pursuit.csv　＿＿＿　MP法の解
■RBS.csv　＿＿＿　RBSの解
                            etc...