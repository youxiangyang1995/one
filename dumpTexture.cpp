void textureToBuffer(int textureId, int x, int y, int width, int height, unsigned char *buffer) {
    // 创建FBO
    GLuint fbo[1];
    glGenFramebuffers(1, fbo);
    // 绑定
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    // 读取数据
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);  // 这里的format和type需要和纹理一致
    // 解绑和释放
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // unbind
    glDeleteFramebuffers(1, fbo);
}

static int dumpImageNum = 0;

void dumpInputBuffer(unsigned char* bufferVaddr, HYY_String name)
{
    // const private_handle_t* pht = (private_handle_t*)nht_in;
    HYY_String filePath = "/sdcard/";
    filePath += name;
    filePath += "/";
    struct stat buffer;
    // 没有检查到这个文件夹，则创建它
    if (stat(filePath.c_str(), &buffer) != 0) {
        if (!mkdir(filePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
            HYY_LOGD("%s create success!", filePath.c_str());
        } else {
            HYY_LOGD("%s create fial!", filePath.c_str());
        }
    } else {
        HYY_LOGD("%s has exist!", filePath.c_str());
    }

    // HYY_String fileName = to_string((int)pht->id);
    HYY_String fileName1 = to_string(dumpImageNum);
    HYY_String path = filePath.append(fileName1);
    path = path.append("_");
    // path = path.append(fileName);
    path = path.append(".yuv");
    FILE* imageFile = fopen(path.c_str(), "wb+");
    if(imageFile) {
        // fwrite(bufferVaddr , 1, 4096*2560*3/2, imageFile);
        fwrite(bufferVaddr , 1, 1920*1080*3/2*4, imageFile);
        fclose(imageFile);
    }
    HYY_LOGI("dumpImageNum %d", dumpImageNum);
    if (dumpImageNum == 20) {
        dumpImageNum = 0;
    } else {
        dumpImageNum ++;
    }
}

void dumpTexture(int textureId, HYY_String name)
{
        unsigned char * tmpTexture = new unsigned char[1920*1080*3/2*4];
        textureToBuffer(textureId,0,0,1920,1080,tmpTexture);
        dumpInputBuffer(tmpTexture, name);

        delete[] tmpTexture;
        tmpTexture = nullptr;
}
