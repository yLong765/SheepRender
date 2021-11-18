//
// Created by long on 2021/11/18.
//

#ifndef SHEEPRENDER_SR_OBJ_LOADER_H
#define SHEEPRENDER_SR_OBJ_LOADER_H

namespace SR {
    typedef struct sr_obj_loader {
        static sr_mesh load(const char *filename) {
            std::ifstream in;
            in.open(filename, std::ifstream::in);
            if (!in.is_open()) {
                std::cerr << "Error: " << strerror(errno);
            }
            std::string line;
            std::vector<vec3> d_v;
            std::vector<vec2> d_vt;
            std::vector<vec3> d_vn;
            std::vector<vec3> d_f;
            while (!in.eof()) {
                getline(in, line);
                std::istringstream iss(line.c_str());
                char trash;
                if (!line.compare(0, 2, "v ")) {
                    iss >> trash;
                    vec3 v;
                    for (int i = 0; i < 3; i++) iss >> v[i];
                    d_v.push_back(v);
                } else if (!line.compare(0, 3, "vt ")) {
                    iss >> trash >> trash;
                    vec2 uv;
                    for (int i = 0; i < 2; i++) iss >> uv[i];
                    d_vt.push_back(uv);
                } else if (!line.compare(0, 3, "vn ")) {
                    iss >> trash >> trash;
                    vec3 n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    d_vn.push_back(n);
                } else if (!line.compare(0, 2, "f ")) {
                    vec3 tmp;
                    iss >> trash;
                    while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                        for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                        d_f.push_back(tmp);
                    }
                }
            }
            std::vector<vec3> vertices;
            std::vector<vec2> uv;
            std::vector<vec3> normals;
            std::vector<int> triangles;
            for (int i = 0; i < d_f.size(); i++) {
                vertices.push_back(d_v[d_f[i][0]]);
                uv.push_back(d_vt[d_f[i][1]]);
                normals.push_back(d_vn[d_f[i][2]]);
                triangles.push_back(i);
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
