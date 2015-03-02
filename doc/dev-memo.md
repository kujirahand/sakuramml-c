# 開発中のメモ

## CC

本来、CCは0-127まで。
しかし、128以降を独自コントロールに割り当てることで、プログラムが簡単になる
そこで、以下のようにする。

130 --- PitchBend(full range)
131 --- PitchBend(half range)


