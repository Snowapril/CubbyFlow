import pyCubbyFlow


def test_face_centered_grid2_Fill():
    a = pyCubbyFlow.FaceCenteredGrid2((10, 10))
    a.Fill((3.0, 4.0))
    for j in range(10):
        for i in range(11):
            assert a.U((i, j)) == 3.0
    for j in range(11):
        for i in range(10):
            assert a.V((i, j)) == 4.0

    def filler(pt):
        return (pt.x, pt.y)

    a.Fill(filler)
    for j in range(10):
        for i in range(11):
            assert a.U((i, j)) == i
    for j in range(11):
        for i in range(10):
            assert a.V((i, j)) == j


def test_face_centered_grid2_for_each():
    a = pyCubbyFlow.FaceCenteredGrid2((10, 10))
    # Workaround for Python 2.x which doesn't support nonlocal
    d = {'ei': 0, 'ej': 0}

    def checkU(idx):
        assert idx[0] == d['ei']
        assert idx[1] == d['ej']
        d['ei'] += 1
        if d['ei'] >= 11:
            d['ei'] = 0
            d['ej'] += 1

    a.ForEachUIndex(checkU)
    d = {'ei': 0, 'ej': 0}

    def checkV(idx):
        assert idx[0] == d['ei']
        assert idx[1] == d['ej']
        d['ei'] += 1
        if d['ei'] >= 10:
            d['ei'] = 0
            d['ej'] += 1

    a.ForEachVIndex(checkV)


def test_face_centered_grid2_serialization():
    a = pyCubbyFlow.FaceCenteredGrid2((10, 10))

    def filler(pt):
        return (pt.x, pt.y)

    a.Fill(filler)

    flatBuffer = a.Serialize()

    b = pyCubbyFlow.FaceCenteredGrid2()
    b.Deserialize(flatBuffer)

    for j in range(10):
        for i in range(11):
            assert b.U((i, j)) == i
    for j in range(11):
        for i in range(10):
            assert b.V((i, j)) == j
