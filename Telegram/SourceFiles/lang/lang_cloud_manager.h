/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "mtproto/sender.h"
#include "base/weak_ptr.h"

namespace MTP {
class Instance;
} // namespace MTP

namespace Lang {

class Instance;
enum class Pack;
struct Language;

Language ParseLanguage(const MTPLangPackLanguage &data);

class CloudManager : public base::has_weak_ptr, private MTP::Sender, private base::Subscriber {
public:
	CloudManager(Instance &langpack, not_null<MTP::Instance*> mtproto);

	using Languages = std::vector<Language>;

	void requestLanguageList();
	const Languages &languageList() const {
		return _languages;
	}
	base::Observable<void> &languageListChanged() {
		return _languagesChanged;
	}
	void requestLangPackDifference(const QString &langId);
	void applyLangPackDifference(const MTPLangPackDifference &difference);
	void setCurrentVersions(int version, int baseVersion);

	void resetToDefault();
	void switchWithWarning(const QString &id);
	void switchToLanguage(const QString &id);
	void switchToLanguage(const Language &data);
	void switchToTestLanguage();
	void setSuggestedLanguage(const QString &langCode);
	QString suggestedLanguage() const {
		return _suggestedLanguage;
	}
	base::Observable<void> &firstLanguageSuggestion() {
		return _firstLanguageSuggestion;
	}

private:
	mtpRequestId &packRequestId(Pack pack);
	mtpRequestId packRequestId(Pack pack) const;
	Pack packTypeFromId(const QString &id) const;
	void requestLangPackDifference(Pack pack);
	bool canApplyWithoutRestart(const QString &id) const;
	void performSwitchToCustom();
	void performSwitch(const Language &data);
	void performSwitchAndRestart(const Language &data);
	void restartAfterSwitch();
	void offerSwitchLangPack();
	bool showOfferSwitchBox();
	Language findOfferedLanguage() const;

	void requestLanguageAndSwitch(const QString &id, bool warning);
	void applyLangPackData(Pack pack, const MTPDlangPackDifference &data);
	void switchLangPackId(const Language &data);
	void changeIdAndReInitConnection(const Language &data);

	Instance &_langpack;
	Languages _languages;
	base::Observable<void> _languagesChanged;
	mtpRequestId _langPackRequestId = 0;
	mtpRequestId _langPackBaseRequestId = 0;
	mtpRequestId _languagesRequestId = 0;

	QString _offerSwitchToId;
	bool _restartAfterSwitch = false;

	QString _suggestedLanguage;
	bool _languageWasSuggested = false;
	base::Observable<void> _firstLanguageSuggestion;

	mtpRequestId _switchingToLanguageRequest = 0;

};

CloudManager &CurrentCloudManager();

} // namespace Lang
