.pragma library

// Find an object with the given name recursively, starting
// at the given object "obj"
function findChild(obj,objectName) {
    for (var i in obj.children) {
        var child = obj.children[i];
        if (child.objectName === objectName) return child;
        var subChild = findChild(child,objectName);
        if (subChild !== undefined) return subChild;
    }
    return undefined;
}
