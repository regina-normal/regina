# A library of routines to determine if a packet is of a particular type.

# Is the given packet an angle structure list?
#
# Return: 1 for true, 0 for false.
def isAngleStructureList(packet):
    return (packet.getPacketType() == regina.NAngleStructureList.packetType)

# Is the given packet a container?
#
# Return: 1 for true, 0 for false.
def isContainer(packet):
    return (packet.getPacketType() == regina.NContainer.packetType)

# Is the given packet a script?
#
# Return: 1 for true, 0 for false.
def isScript(packet):
    return (packet.getPacketType() == regina.NScript.packetType)

# Is the given packet a surface filter?
#
# Return: 1 for true, 0 for false.
def isSurfaceFilter(packet):
    return (packet.getPacketType() == regina.NSurfaceFilter.packetType)

# Is the given packet a normal surface list?
#
# Return: 1 for true, 0 for false.
def isSurfaceList(packet):
    return (packet.getPacketType() == regina.NNormalSurfaceList.packetType)

# Is the given packet a text packet?
#
# Return: 1 for true, 0 for false.
def isText(packet):
    return (packet.getPacketType() == regina.NText.packetType)

# Is the given packet a triangulation?
#
# Return: 1 for true, 0 for false.
def isTriangulation(packet):
    return (packet.getPacketType() == regina.NTriangulation.packetType)

