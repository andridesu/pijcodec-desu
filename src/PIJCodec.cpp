#include "PIJCodec.hpp"
#include <fstream>
#include <vector>
#include <zlib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

void writeBigEndian(std::ostream& out, uint32_t v) {
    for (int i = 3; i >= 0; --i)
        out.put(char((v >> (i*8)) & 0xFF));
}
uint32_t readBigEndian(std::istream& in) {
    uint32_t v=0; char b;
    for(int i=0;i<4;++i){ in.get(b); v=(v<<8)|uint8_t(b); }
    return v;
}
uint32_t computeCRC(const std::vector<uint8_t>& d,uint32_t s=0){
    return crc32(s,d.data(),d.size());
}
std::vector<uint8_t> encodeJPEG(const cv::Mat& img,int q){
    std::vector<uint8_t> buf; cv::imencode(".jpg",img,buf,{cv::IMWRITE_JPEG_QUALITY,q}); return buf;
}
std::vector<uint8_t> encodePNG(const cv::Mat& img){
    std::vector<uint8_t> buf; cv::imencode(".png",img,buf); return buf;
}
cv::Mat decodeImage(const std::vector<uint8_t>& d){
    return cv::imdecode(d,cv::IMREAD_COLOR);
}

} // anon

void PIJCodec::encode(
    const cv::Mat& input,
    const std::vector<std::vector<int>>& losslessBoxes,
    std::ostream& out,
    int pad,int jpegQuality,int zlibLevel
) {
    if(input.empty()) throw std::runtime_error("Empty input");
    std::vector<std::vector<int>> boxes;
    std::vector<std::vector<uint8_t>> regions;
    for(auto &b: losslessBoxes) {
        int x1=std::max(0,b[0]-pad), y1=std::max(0,b[1]-pad);
        int x2=std::min(input.cols,b[2]+pad), y2=std::min(input.rows,b[3]+pad);
        boxes.push_back({x1,y1,x2,y2});
        regions.push_back(encodePNG(input(cv::Rect(x1,y1,x2-x1,y2-y1))));
    }
    cv::Mat bg=input.clone();
    for(auto &b: losslessBoxes)
        cv::rectangle(bg,cv::Rect(b[0],b[1],b[2]-b[0],b[3]-b[1]),cv::Scalar(0),cv::FILLED);
    auto jpegData=encodeJPEG(bg,jpegQuality);
    json md={ {"v",1}, {"b",boxes} };
    auto s=md.dump();
    std::vector<uint8_t> cmp(s.size()*2);
    uLongf l=cmp.size(); compress2(cmp.data(),&l,(Bytef*)s.data(),s.size(),zlibLevel);
    cmp.resize(l);
    uint32_t crc=computeCRC(jpegData);
    for(auto &r:regions) crc=computeCRC(r,crc);
    crc=computeCRC(cmp,crc);
    out.write("PIJ\x01",4);
    writeBigEndian(out,jpegData.size()); out.write((char*)jpegData.data(),jpegData.size());
    writeBigEndian(out,regions.size());
    for(auto &r:regions){ writeBigEndian(out,r.size()); out.write((char*)r.data(),r.size()); }
    writeBigEndian(out,cmp.size()); out.write((char*)cmp.data(),cmp.size());
    writeBigEndian(out,crc);
}

cv::Mat PIJCodec::decode(std::istream& in) {
    char m[4]; in.read(m,4);
    if(std::string(m,4)!="PIJ\x01") throw std::runtime_error("Bad magic");
    uint32_t jl=readBigEndian(in);
    std::vector<uint8_t> j(jl); in.read((char*)j.data(),jl);
    uint32_t n=readBigEndian(in);
    std::vector<std::vector<uint8_t>> reg(n);
    for(uint32_t i=0;i<n;i++){ uint32_t l=readBigEndian(in); reg[i].resize(l); in.read((char*)reg[i].data(),l); }
    uint32_t ml=readBigEndian(in);
    std::vector<uint8_t> cm(ml); in.read((char*)cm.data(),ml);
    uint32_t sc=readBigEndian(in), cc=computeCRC(j);
    for(auto &r:reg) cc=computeCRC(r,cc);
    cc=computeCRC(cm,cc);
    if(cc!=sc) throw std::runtime_error("CRC");
    uLongf ul=4096; std::vector<uint8_t> um(ul);
    uncompress(um.data(),&ul,cm.data(),ml);
    
    auto md = json::parse(std::string((char*)um.data(),ul));
    cv::Mat img = decodeImage(j);
    
    for(size_t i=0; i<reg.size(); i++) {
        auto &b = md["b"][i];
        auto p = decodeImage(reg[i]);
        p.copyTo(img(cv::Rect(b[0],b[1],p.cols,p.rows)));
    }
    
    return img;
}
