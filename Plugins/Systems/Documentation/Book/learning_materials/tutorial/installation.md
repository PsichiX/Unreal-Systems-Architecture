# Installation

Until plugin gets accepted to Unreal Engine Marketplace, we need to install plugin
from GitHub.

1. Download plugin from GitHub Release page:
    https://github.com/PsichiX/Unreal-Systems-Architecture/releases
    (core plugin is in `Systems-Architecture.zip` archive file).

1. Make sure project folder structure contains `Plugins` folder in its root (next
    to `Content` and `Source` folders), then unzip its content into `Plugins` folder.

1. Launch editor and confirm plugin is properly by trying to create new data asset
    of [`class: USystemsPipeline`]() type:

    ![Create pipeline data asset](../../../assets/systems_pipeline_create_data_asset.png)
