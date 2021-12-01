//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_OBJ_LOADER_H
#define SHEEPRENDER_SR_OBJ_LOADER_H

namespace SR {
    // obj模型数据加载
    typedef struct sr_obj_loader {
        static sr_mesh load(const char *filename) {
            std::ifstream in;
            in.open(filename, std::ifstream::in);
            if (!in.is_open()) {
                std::cerr << "Error: " << strerror(errno);
            }
            std::string line;
            std::vector<vec3f> d_v;
            std::vector<vec2f> d_vt;
            std::vector<vec3f> d_vn;
            int d_f_count = 0;

            std::vector<vec3f> vertices;
            std::vector<vec2f> uv;
            std::vector<vec3f> normals;
            std::vector<int> triangles;

            while (!in.eof()) {
                getline(in, line);
                std::istringstream iss(line.c_str());
                char trash;
                if (!line.compare(0, 2, "v ")) {
                    iss >> trash;
                    vec3f v;
                    for (int i = 0; i < 3; i++) iss >> v[i];
                    d_v.push_back(v);
                } else if (!line.compare(0, 3, "vt ")) {
                    iss >> trash >> trash;
                    vec2f uv;
                    for (int i = 0; i < 2; i++) iss >> uv[i];
                    d_vt.push_back(uv);
                } else if (!line.compare(0, 3, "vn ")) {
                    iss >> trash >> trash;
                    vec3f n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    d_vn.push_back(n);
                } else if (!line.compare(0, 2, "f ")) {
                    std::vector<vec3f> d_f;
                    vec3f tmp;
                    bool has_v = !d_v.empty();
                    bool has_vt = !d_vt.empty();
                    bool has_vn = !d_vn.empty();
                    iss >> trash;
                    while (iss >> tmp[0] >> trash) {
                        if (has_vt) iss >> tmp[1];
                        iss >> trash;
                        if (has_vn) iss >> tmp[2];
                        for (int i = 0; i < 3; tmp[i]--, i++);
                        d_f.push_back(tmp);
                    }
                    if (d_f.size() == 3) {
                        for (int i = 0; i < 3; i++) {
                            if (has_v) vertices.push_back(d_v[d_f[i][0]]);
                            if (has_vt) uv.push_back(d_vt[d_f[i][1]]);
                            if (has_vn) normals.push_back(d_vn[d_f[i][2]]);
                            triangles.push_back(d_f_count);
                            d_f_count++;
                        }
                    } else {
                        triangles.push_back(d_f_count);
                        triangles.push_back(d_f_count + 1);
                        triangles.push_back(d_f_count + 2);
                        triangles.push_back(d_f_count + 2);
                        triangles.push_back(d_f_count + 3);
                        triangles.push_back(d_f_count);
                        for (int i = 0; i < 4; i++) {
                            if (has_v) vertices.push_back(d_v[d_f[i][0]]);
                            if (has_vt) uv.push_back(d_vt[d_f[i][1]]);
                            if (has_vn) normals.push_back(d_vn[d_f[i][2]]);
                            d_f_count++;
                        }
                    }
                }
            }
            sr_mesh mesh;
            mesh.vertices = vertices;
            mesh.uv = uv;
            mesh.normals = normals;
            mesh.triangles = triangles;
            printf("model loading finish\n");
            return mesh;
        }
    } obj_loader;
}

#endif //SHEEPRENDER_SR_OBJ_LOADER_H
