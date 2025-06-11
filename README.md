# HW8
## Q1: Immediate Mode 구현 설명
### 렌더링 방식

glBegin(GL_TRIANGLES)와 glEnd() 사이에서 정점(glVertex3f)과 노멀(glNormal3f) 정보를 직접 넘겨주는 즉시 모드(Immediate Mode) 방식

삼각형마다 3개의 정점을 직접 순회하면서 렌더링

### 구현 구조

glBegin(GL_TRIANGLES);
for (const auto& tri : gTriangles) {
    for (int j = 0; j < 3; ++j) {
        unsigned int idx = tri.indices[j];
        Vector3 n = gNormals[idx];
        Vector3 v = gPositions[idx];
        glNormal3f(n.x, n.y, n.z);
        glVertex3f(v.x, v.y, v.z);
    }
}
glEnd();

### 장점

구현이 단순하고 직관적

작은 데이터셋에서는 충분히 쓸 수 있음

### 단점

GPU와의 통신이 빈번하여 매우 비효율적

매 프레임마다 정점 데이터를 CPU에서 GPU로 다시 보내야 함

더 이상 OpenGL Core Profile에서는 지원되지 않음

## Q2: Vertex Array 구현 설명
### 렌더링 방식

정점 배열(Vertex Array), 노멀 배열, 인덱스 배열을 생성한 뒤, 한 번에 glDrawElements()로 그리는 방식

정점/노멀 데이터를 GPU 메모리에 저장하고 재사용 가능

### 구현 구조 요약

glGenBuffers(...); // VBO/IBO 생성
glBufferData(...); // 데이터 업로드

glBindVertexArray(...);
glVertexPointer(...);
glNormalPointer(...);

glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);

### 특이점

Q1과 동일한 출력 결과를 위해 정점과 노멀을 삼각형마다 중복 저장
(즉, 공유 정점을 쓰지 않고 완전히 펼쳐서 저장)

### 장점
GPU 자원을 효율적으로 사용 → 매우 높은 렌더링 성능

정점 데이터 재사용 가능

실제 그래픽스 파이프라인 구조와 더 유사함

### 단점

코드 구조가 복잡해지고, 버퍼 생성을 신경 써야 함

초기 구현 난이도는 Q1보다 높음
