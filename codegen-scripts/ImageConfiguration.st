"the bae configuration for loading VMMaker can be found under
../codegen-scripts/LoadVMMaker.st"

"Open workspace with example Configurations"
Workspace openContents: (String streamContents: [ :stream|
        stream << '"Configuration for OSX ---------------------------------------------------"'; cr.
        stream << 'CogCocoaIOSConfig new'; cr.
        stream << '    addExternalPlugins: #( FT2Plugin );'; cr.
        stream << '    addInternalPlugins: #( UnixOSProcessPlugin );'; cr.
        stream << '    "generateForDebug;"'; cr.
        stream << '    generateSources; generate.'; cr; cr.

        stream << '"Configuration for Linux -------------------------------------------------"'; cr.
        stream << 'CogUnixConfig new'; cr.
        stream << '    addExternalPlugins: #( FT2Plugin );'; cr.
        stream << '    addInternalPlugins: #( UnixOSProcessPlugin );'; cr.
        stream << '    "generateForDebug;"'; cr.
        stream << '    generateSources; generate.'; cr; cr.

        stream << '"Configuration for Windows -----------------------------------------------"'; cr.
        stream << 'CogWindowsConfig new'; cr.
        stream << '    addExternalPlugins: #( FT2Plugin );'; cr.
        stream << '    addInternalPlugins: #( Win32OSProcessPlugin );'; cr.
        stream << '    "generateForDebug;"'; cr.
        stream << '    generateSources; generate.'; cr; cr.

        stream << '"Configuration for iOS-devices -------------------------------------------"'; cr.
        stream << 'StackIPhoneConfig new'; cr.
        stream << '    addExternalPlugins: #( FT2Plugin );'; cr.
        stream << '    "generateDebug;"'; cr.
        stream << '    generateSources; generate.'; cr; cr.

        stream << '"A list of all plugins ---------------------------------------------------"'; cr.
        stream << '(SmartSyntaxInterpreterPlugin allSubclasses select: #shouldBeTranslated) inspect.'; cr.
        stream << '"Or send #internalPlugins or #externalPlugins to the chosen onfiguration"'.
        
        stream << '"send #internalPlugins or #externalPlugins to see the currently used plugins of your onfiguration"'; cr.
        stream << 'CogUnixConfig new internalPlugins'; cr.
        stream << 'CogUnixConfig new externalPlugins'; cr.
    ]).

"save the image and quit"
Smalltalk snapshot: true andQuit: true.
