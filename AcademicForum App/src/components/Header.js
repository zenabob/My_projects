import React, {useState} from 'react';
import {View, Text, TouchableOpacity, ScrollView, StyleSheet, Modal, SafeAreaView, Dimensions} from 'react-native';
import {FontAwesome} from '@expo/vector-icons';
import {useTranslation} from 'react-i18next';

const Header = ({navigation}) => {
    const {t, i18n} = useTranslation();
    const [isSidebarOpen, setSidebarOpen] = useState(false);

    const handleLanguageChange = (language) => {
        i18n.changeLanguage(language);
    };

    const handleMenuPress = () => {
        setSidebarOpen(!isSidebarOpen);
    };

    return (
        <SafeAreaView>
            <View style={styles.header}>
                <TouchableOpacity onPress={handleMenuPress} style={styles.menuButton}>
                    {isSidebarOpen ? (
                        <FontAwesome name="times" size={24} color="black"/>
                    ) : (
                        <FontAwesome name="bars" size={24} color="black"/>
                    )}
                </TouchableOpacity>

                <View style={styles.languageContainer}>
                    <TouchableOpacity
                        onPress={() => handleLanguageChange('EN')}
                        style={styles.languageButton}
                    >
                        <Text style={styles.languageText}>EN</Text>
                    </TouchableOpacity>
                    <TouchableOpacity
                        onPress={() => handleLanguageChange('AR')}
                        style={styles.languageButton}
                    >
                        <Text style={styles.languageText}>AR</Text>
                    </TouchableOpacity>
                    <TouchableOpacity
                        onPress={() => handleLanguageChange('HE')}
                        style={styles.languageButton}
                    >
                        <Text style={styles.languageText}>HE</Text>
                    </TouchableOpacity>
                </View>

                <Modal visible={isSidebarOpen} animationType="none" transparent={true} onRequestClose={handleMenuPress}>
                    <TouchableOpacity
                        style={styles.modalOverlay}
                        activeOpacity={1}
                        onPress={handleMenuPress}
                    >
                        <ScrollView>
                            <TouchableOpacity
                                onPress={() => {
                                    handleMenuPress();
                                    navigation.navigate('SearchScreen');
                                }}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>{t('navbar.search')}</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => {
                                    handleMenuPress();
                                    navigation.navigate('PodcastScreen');
                                }}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>{t('navbar.podcast')}</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => {
                                    handleMenuPress();
                                    navigation.navigate('ContactUsScreen');
                                }}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>{t('navbar.contact')}</Text>
                            </TouchableOpacity>
                        </ScrollView>
                    </TouchableOpacity>
                </Modal>
            </View>
        </SafeAreaView>
    );
};

export default Header;

const styles = StyleSheet.create({
    header: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        paddingHorizontal: 16,
        paddingTop: 30,
        zIndex: 1,
    },
    menuButton: {
        padding: 8,
    },
    languageContainer: {
        flexDirection: 'row',
    },
    languageButton: {
        paddingHorizontal: 8,
    },
    languageText: {
        fontSize: 16,
    },
    modalOverlay: {
        position: 'absolute',
        backgroundColor: 'rgba(225, 225, 225, 0.5)',
        marginTop: '30%',
        width: '70%',
        height: '100%', // Set the height to cover the entire screen
        left: 0, // Position the sidebar to the left
        top: 0, // Position the sidebar to the top
        zIndex: 0,
    },
    sidebarButton: {
        padding: 16,
        borderBottomWidth: 1,
        borderBottomColor: 'lightgray',
    },
    sidebarButtonText: {
        fontSize: 16,
    },
});
