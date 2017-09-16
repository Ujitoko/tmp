= GANによるいらすとや画像生成

== はじめに

Generative Adversarial Network(GAN)というニューラルネットのモデルがあります。
このモデルはYann LeCunに，ここ10年で最も興味深いアイデアと言わしめ@<bib>{lecun}，学術的にも注目を集めています．
実際，arXivを覗いてみると、GAN関連の論文が近年急速に増えており、動向を追うのに一苦労です。
GANの応用は広く、写真に対して有名画家の画風を加えることが可能なcycleGAN@<bib>{cycleGAN}や、アカデミアの外だと、2017年8月には鮮明な美少女画像を生成可能なサービスが公開され衝撃が走りました@<bib>{animeGAN}.これらのGANの関連論文についてはawesomeシリーズをチェックしておくのがお薦めです。
本稿では，GANの概略と，その後で@<bib>{animeGAN}の手法の構成要素を一つ一つ説明し、最後に「いらすとや」の画像に対して適用した結果を紹介します．

#@#　以下で紹介するDRAGANの実装については公開しています．
#@#　@<href>{https://github.com/Ujitoko/GAN}
#@#　(本稿で紹介しきれなかったunrolled GAN，BEGAN，WGAN，Cramer-GANについても同様に公開)

== GANの基本

GANは，内部に2つのネットワークを持ち，これら2つのネットワークを競争させるように学習を進めていきます．これら2つのネットワークの関係は，貨幣の偽造者(counterfeiter)と，それを見抜く警察(police)によく例えられます．
偽造者の目的は，本物に似せた偽貨幣を作って警察を欺くことです．
一方，警察の目的は，真の貨幣と偽貨幣の集合から真の貨幣のみ区別することです．
この対立する目的を持つネットワーク同士を競わせる学習プロセスのため，Adversarial（敵対的） Processと呼ばれています．

GANでは全体のネットワークの中に，警察と偽造者のネットワークが組み込まれる形になっているのですが，
それぞれDiscriminatorとGeneratorと呼びます．
Generatorは画像を生成します．どのように生成するかというと，一様分布や正規分布などからサンプリングしたノイズベクトル z をアップサンプリングして画像化します．
一方，Discriminatorは分類問題を解きます．Generatorの生成した偽の画像と，真の画像を分類するのがDiscriminatorの目的です．
この2つのネットワークを交互に学習していけば、Generatorは本物のデータに近いデータを生成するようになります．

このGANの変種は数多く提案されています。
各GANにはそれぞれ特徴があります。
例えば、鮮明な画像を生成できるGANがあったり、分布の多様性を保って画像を生成できるGANがあったりします。

本稿では、@<bib>{animeGAN}が提案したGANについて、
その構成要素として、DRAGAN、SRResNet, PixelCNNを順に紹介していきます。

== 目的関数：DRAGAN

DRAGANは2017年5月にarxivに投稿されました。
WGAN-gp@<bib>{WGAN-gp}やCramerGAN@<bib>{CramerGAN}などが投稿された直後であったことで、
当時、あまり注目はされていませんでした。
しかしながら、ChainerのGANライブラリに実装されたり、
驚異的な質の画像を生成できる@<bib>{animeGAN}で採用されているため、
その価値は注目度よりもずっと大きかったのでしょう。

#@# あとでリプシッツ性がなぜ必要かを付け加える
GANの性能は、Discriminatorの性質に依存するという言説があります。
特にDiscriminatorに対してリプシッツ性の制約を加えたGANが注目を集めています。

リプシッツ性の制約とは、簡単なイメージでは、
どの場所でも、関数の傾きの絶対値がある値を超えないことです。
この制約をDiscriminatorに加えることで、Generatorの生成した偽の画像と、真の画像の分布を安定に近づけることができます。

Discriminatorにリプシッツ性を施す実現手段は様々提案されています。
例えば、LSGAN@<bib>{LSGAN}ではweight decayによりリプシッツ性を担保しています。
また、WGAN@<bib>{WGAN}ではweight clippingにより制約をかけています。
しかしながら、これらの制約は強すぎて、Generatorの表現力を抑えてしまうという欠点があります。
そこでDRAGANでは、訓練に使う真のデータに近いデータに限って、
勾配を1に近づけることで、Discriminatorの目的関数を線形に近づける操作を行っています。

実装としてはシンプルです。
1. 真の画像のミニバッチを作成
2. 真の画像に外乱を加えたミニバッチを作成
3. 1.と2.の内分点をランダムに1つずつ選択
4. 3.で選択した内分点に関してDiscriminatorに掛けて、その勾配が1に近づくように目的関数を構成

その他の面では、もともとのGANの実装をそのまま使えます。

== ネットワークの工夫1： SRResNet

SRResNet

== ネットワークの工夫2: PixelCNN



== おわりに

GANの論文やその実装の公開により，手法の応用のハードルが益々下がっています．
ただGAN自体にはまだまだ課題も多く残っています．
生成データの制御方法や生成データの評価方法等はデファクトが今はまだ存在していません．
今後もGAN関連の論文の動向を注視しつつ，広く応用していきたいと考えています．